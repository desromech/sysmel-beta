#include "sysmel/BaseCompiler/SObjectSchema.hpp"
#include <stdexcept>
#include <sstream>
#include <fstream>

#include <iostream>

namespace SysmelMoebius
{
namespace Compiler
{
namespace SObjectSchema
{

struct SchemaParsingContext
{
    SchemaPtr schema;
    std::string namespacePrefix;
    std::string activeNamespace;
    std::string directory;
};

static void parseSchemaFromFileNamed(const std::string &fileName, SchemaParsingContext &context);

static void parseSchemaNamespaceFrom(const DOM::Array &form, SchemaParsingContext &context)
{
    context.activeNamespace = context.namespacePrefix;
    for(size_t i = 1; i < form.size(); ++i)
    {
        auto next = std::get<DOM::Symbol> (form[i]).value;
        if(!context.activeNamespace.empty())
            context.activeNamespace += " ";
        context.activeNamespace += next;
    }
}

static void parseSchemaLoadFrom(const DOM::Array &form, SchemaParsingContext &context)
{
    auto fileName = std::get<DOM::String> (form[1]).value;
    auto fullFileName = context.directory + fileName;

    auto subContext = context;
    subContext.namespacePrefix = subContext.activeNamespace;
    parseSchemaFromFileNamed(fullFileName, subContext);
}

static SchemaMethodExpressionPtr parseSchemaMethodExpressionFrom(const DOM::Value &expressionValue, SchemaParsingContext &context);

typedef SchemaMethodExpressionPtr (*ExpressionFormParser) (const DOM::Array &form, SchemaParsingContext &context);
static std::map<std::string, ExpressionFormParser> expressionFormParserDispatchTable{
    {"SelfType", +[](const DOM::Array &, SchemaParsingContext &) -> SchemaMethodExpressionPtr {
        return std::make_shared<SchemaMethodSelfTypeExpression> ();
    }},

    {"typeToCTypeName", +[](const DOM::Array &form, SchemaParsingContext &context) -> SchemaMethodExpressionPtr {
        auto result = std::make_shared<SchemaTypeToCTypeNameExpression> ();
        result->typeExpression = parseSchemaMethodExpressionFrom(form[1], context);
        return result;
    }},

};

static SchemaMethodExpressionPtr parseSchemaMethodExpressionFrom(const DOM::Value &expressionValue, SchemaParsingContext &context)
{
    struct Visitor
    {
        SchemaParsingContext &context;

        SchemaMethodExpressionPtr operator()(const DOM::Void &)
        {
            return std::make_shared<SchemaVoidLiteralValue> ();
        }

        SchemaMethodExpressionPtr operator()(const DOM::Nil &)
        {
            return std::make_shared<SchemaNilLiteralValue> ();
        }

        SchemaMethodExpressionPtr operator()(const DOM::True &)
        {
            return std::make_shared<SchemaTrueLiteralValue> ();
        }

        SchemaMethodExpressionPtr operator()(const DOM::False &)
        {
            return std::make_shared<SchemaFalseLiteralValue> ();
        }

        SchemaMethodExpressionPtr operator()(const DOM::Integer &value)
        {
            auto schemaValue = std::make_shared<SchemaIntegerLiteralValue> ();
            schemaValue->value = value;
            return schemaValue;
        }

        SchemaMethodExpressionPtr operator()(const DOM::Float &value)
        {
            auto schemaValue = std::make_shared<SchemaFloatLiteralValue> ();
            schemaValue->value = value;
            return schemaValue;
        }

        SchemaMethodExpressionPtr operator()(const DOM::Character &value)
        {
            auto schemaValue = std::make_shared<SchemaCharacterLiteralValue> ();
            schemaValue->value = value;
            return schemaValue;
        }

        SchemaMethodExpressionPtr operator()(const DOM::String &value)
        {
            auto schemaValue = std::make_shared<SchemaStringLiteralValue> ();
            schemaValue->value = value;
            return schemaValue;
        }

        SchemaMethodExpressionPtr operator()(const DOM::Symbol &value)
        {
            auto schemaValue = std::make_shared<SchemaSymbolLiteralValue> ();
            schemaValue->value = value;
            return schemaValue;
        }

        SchemaMethodExpressionPtr operator()(const DOM::Array &form)
        {
            auto formName = std::get<DOM::Symbol> (form[0]).value;
            auto formParserIt = expressionFormParserDispatchTable.find(formName);
            if(formParserIt == expressionFormParserDispatchTable.end())
                throw std::runtime_error("Unsupported expression form #" + formName);

            return formParserIt->second(form, context);
        }

        SchemaMethodExpressionPtr operator()(const DOM::Dictionary &)
        {
            return nullptr;
        }
    };

    return std::visit(Visitor{context}, expressionValue);
}

static void parseSchemaMethodFrom(const SchemaMethodPtr &method, const DOM::Value &methodValue, SchemaParsingContext &context)
{
    method->valueExpression = parseSchemaMethodExpressionFrom(methodValue, context);

}

static void parseSchemaTypeFrom(const DOM::Array &form, SchemaParsingContext &context)
{
    auto type = std::make_shared<SchemaType> ();
    type->fullNamespace = context.activeNamespace;
    type->name = std::get<DOM::Symbol> (form[1]).value;
    if(std::holds_alternative<DOM::Symbol> (form[2]))
        type->superTypeName = std::get<DOM::Symbol> (form[2]).value;

    for(size_t i = 3; i < form.size(); ++i)
    {
        auto keyword = std::get<DOM::Symbol> (form[i]).value;
        if(keyword == "sysmelName")
        {
            type->sysmelName = std::get<DOM::Symbol> (form[++i]).value;
            continue;
        }

        if(keyword == "methods")
        {
            auto methods = std::get<DOM::Dictionary> (form[++i]);
            for(auto &[key, valueExpression] : methods.elements)
            {
                auto method = std::make_shared<SchemaMethod> ();
                method->selector = std::get<DOM::Symbol> (key).value;
                parseSchemaMethodFrom(method, valueExpression, context);
                type->methodDictionary.insert(std::make_pair(method->selector, method));
            }
            continue;
        }

        throw std::runtime_error("Unsupport type spec keyword " + keyword);
    }

    context.schema->addEntity(type);
}

typedef void (*SchemaFormParser) (const DOM::Array &form, SchemaParsingContext &context);
static std::map<std::string, SchemaFormParser> formParserDispatchTable{
    {"namespace", parseSchemaNamespaceFrom},
    {"load", parseSchemaLoadFrom},
    {"type", parseSchemaTypeFrom},
};

static void parseSchemaElementFrom(const DOM::Array &form, SchemaParsingContext &context)
{
    auto formName = std::get<DOM::Symbol> (form[0]).value;
    auto formParserIt = formParserDispatchTable.find(formName);
    if(formParserIt == formParserDispatchTable.end())
        throw std::runtime_error("Unsupported schema form #" + formName);

    formParserIt->second(form, context);
}

static void parseSchemaFromDOM(const DOM::Array &dom, SchemaParsingContext &context)
{
    for(auto &element : dom)
        parseSchemaElementFrom(std::get<DOM::Array> (element), context);
}

static void parseSchemaFromFileNamed(const std::string &fileName, SchemaParsingContext &context)
{
    std::ifstream in(fileName);
    if(!in.good())
        throw std::runtime_error("Failed to open file \"" + fileName + "\".");

    auto schema = std::make_shared<Schema> ();
    auto content = std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());
    auto domParseResult = DOM::parseWithSysmelSyntax(std::make_shared<SourceCollection> (content, fileName));

    if(std::holds_alternative<DOM::ParseError> (domParseResult))
    {
        auto error = std::get<DOM::ParseError> (domParseResult);
        std::ostringstream out;
        out << error.location << ": " << error.errorMessage << std::ends;
        throw std::runtime_error(out.str());
    }

    parseSchemaFromDOM(std::get<DOM::Array> (std::get<DOM::Value> (domParseResult)), context);
}

SchemaPtr parseSchemaFromFileNamed(const std::string &fileName)
{
    SchemaParsingContext parsingContext{};
    parsingContext.schema = std::make_shared<Schema> ();
    parsingContext.directory = "./";

    // FIXME: Move this method into a separate utility library.
    auto separator = fileName.find_last_of("/\\");
    if(separator != std::string::npos)
        parsingContext.directory = fileName.substr(0, separator + 1);

    // Parse the schema from the file.
    parseSchemaFromFileNamed(fileName, parsingContext);

    return parsingContext.schema;
}

} // End of namespace SObjectSchema
} // End of namespace Compiler
} // End of namespace SysmelMoebius
