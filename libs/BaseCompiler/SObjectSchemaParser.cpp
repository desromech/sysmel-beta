#include "sysmel/BaseCompiler/SObjectSchema.hpp"
#include "sysmel/BaseCompiler/DOM.hpp"
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

static void parseSchemaTypeFrom(const DOM::Array &form, SchemaParsingContext &context)
{
    if(form.size() < 4 || form.size() > 5)
    {
        std::ostringstream error;
        error << "Unsupported type specification: " << form << std::ends;
        throw std::runtime_error(error.str());
    }

    auto type = std::make_shared<SchemaType> ();
    type->fullNamespace = context.activeNamespace;
    type->name = std::get<DOM::Symbol> (form[1]).value;
    if(std::holds_alternative<DOM::Symbol> (form[2]))
        type->superTypeName = std::get<DOM::Symbol> (form[2]).value;

    if(form.size() >= 5)
        type->sysmelName = std::get<DOM::Symbol> (form[3]).value;
    else
        type->sysmelName = type->name;

    type->methods = std::get<DOM::Dictionary> (form.elements.back());
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

    parseSchemaFromFileNamed(fileName, parsingContext);

    return parsingContext.schema;
}

} // End of namespace SObjectSchema
} // End of namespace Compiler
} // End of namespace SysmelMoebius
