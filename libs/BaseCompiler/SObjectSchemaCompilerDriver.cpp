#include "sysmel/BaseCompiler/SObjectSchema.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace SysmelMoebius
{
namespace Compiler
{
namespace SObjectSchema
{

struct CppSchemaCodeEmitter : SchemaEntityVisitor
{
    CppSchemaCodeEmitter(SchemaPtr cschema, std::ostream &cout)
        : schema(cschema), out(cout) {}


    SchemaType *selfType = nullptr;

    void beginType(SchemaType &entity)
    {
        out << "struct " << entity.name;
        if(!entity.superTypeName.empty())
            out << " : " << entity.superTypeName;
        out << "\n";
        out << "{\n";
        out << "\ttypedef " << entity.name << " SelfType;\n";
        out << "\n";

        selfType = &entity;
        for (auto &[key, value] : entity.methodDictionary)
        {
            visitEntity(*value);
        }
    }

    void endType(SchemaType &)
    {
        out << "};\n";
        out << "\n";
    }

    void declareType(SchemaType &entity)
    {
        setActiveNamespace(entity.fullNamespace);

        out << "struct " << entity.name << ';';
        if(entity.superType)
            out << " // supertype " << entity.superType->name;
        out << '\n';
    }

    virtual std::any visitType(SchemaType &entity) override
    {
        setActiveNamespace(entity.fullNamespace);

        beginType(entity);
        endType(entity);

        return std::any();
    }

    void beginMethod(SchemaMethod &method)
    {
        out << "\tvirtual " << method.returnType.lock()->ctype << " " << method.selector << "()";
        if(method.isOverride)
            out << " override";

        out << "\n";
        out << "\t{\n";
    }

    void endMethod(SchemaMethod &)
    {
        out << "\t}\n";
        out << "\n";
    }

    virtual std::any visitMethod(SchemaMethod &method) override
    {
        beginMethod(method);

        auto expression = std::any_cast<std::string> (visitEntity(*method.valueExpression));
        out << "\t\t";

        if(method.returnType.lock()->ctype != "void")
            out << "return ";
        out << expression << ";\n";
        endMethod(method);
        return std::any();
    }

    virtual std::any visitTypeToCTypeNameExpression(SchemaTypeToCTypeNameExpression &expression) override
    {
        auto rawTypeName = expression.typeExpression->isSelfTypeExpression() ? selfType->name :
            std::any_cast<std::string> (visitEntity(*expression.typeExpression));

        return std::string("\"" + rawTypeName + "\"");
    }

    virtual std::any visitSelfTypeExpression(SchemaMethodSelfTypeExpression &) override
    {
        return std::string("SelfType");
    }

    virtual std::any visitVoidLiteralValue(SchemaVoidLiteralValue &) override
    {
        return std::string();
    }

    virtual std::any visitNilLiteralValue(SchemaNilLiteralValue &) override
    {
        return std::string("nullptr");
    }

    virtual std::any visitFalseLiteralValue(SchemaFalseLiteralValue &) override
    {
        return std::string("false");
    }

    virtual std::any visitTrueLiteralValue(SchemaTrueLiteralValue &) override
    {
        return std::string("true");
    }

    virtual std::any visitIntegerLiteralValue(SchemaIntegerLiteralValue &value) override
    {
        std::ostringstream out;
        out << value.value << std::ends;
        return std::string(out.str());
    }

    virtual std::any visitFloatLiteralValue(SchemaFloatLiteralValue &value) override
    {
        std::ostringstream out;
        out << value.value << std::ends;
        return std::string(out.str());
    }

    virtual std::any visitCharacterLiteralValue(SchemaCharacterLiteralValue &value) override
    {
        std::ostringstream out;
        out << value.value << std::ends;
        return std::string(out.str());
    }

    virtual std::any visitStringLiteralValue(SchemaStringLiteralValue &value) override
    {
        std::ostringstream out;
        out << '"' << value.value << '"' << std::ends;
        return std::string(out.str());
    }

    virtual std::any visitSymbolLiteralValue(SchemaSymbolLiteralValue &value) override
    {
        std::ostringstream out;
        out << '"' << value.value << '"' << std::ends;
        return std::string(out.str());
    }

    void setActiveNamespace(const std::string &newActiveNamespace)
    {
        if(activeNamespace == newActiveNamespace)
            return;

        if(!activeNamespace.empty())
        {
            out << "} /* ";
            for(auto &c : activeNamespace)
            {
                if(c == ' ')
                    out << " */ } /* ";
                else
                    out << c;
            }

            out << " */ \n";
        }
        activeNamespace = newActiveNamespace;

        if(activeNamespace.empty())
            return;

        out << "namespace ";
        for(auto &c : activeNamespace)
        {
            if(c == ' ')
            {
                out << " { namespace ";
            }
            else
            {
                out << c;
            }
        }

        out << '\n';
    }

    void processSchema()
    {
        // Process the types in the sorted order.
        for(auto &type : schema->sortedTypes)
        {
            declareType(*type);
        }
        out << '\n';

        // Perform the actual emission of the types.
        for(auto &type : schema->sortedTypes)
        {
            visitEntity(*type);
        }

        setActiveNamespace("");
    }


    SchemaPtr schema;
    std::ostream &out;
    std::string activeNamespace;
    std::string indent = "    ";
};

} // End of namespace SObjectSchema
} // End of namespace Compiler
} // End of namespace SysmelMoebius

using namespace SysmelMoebius::Compiler::SObjectSchema;

int main(int argc, const char *argv[])
{
    if(argc < 3)
    {
        std::cout << argv[0] << " <input sschema> <output header file>" << std::endl;
        return 0;
    }

    std::string inputSSchemaFileName = argv[1];
    std::string outputHeaderFileName = argv[2];

    SchemaPtr parsedSchema;
    try
    {
        parsedSchema = parseSchemaFromFileNamed(inputSSchemaFileName);
        if(!parsedSchema)
            return 1;

        semanticAnalyzeSchema(parsedSchema);
    }
    catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if(outputHeaderFileName == "-")
    {
        CppSchemaCodeEmitter(parsedSchema, std::cout).processSchema();
    }
    else
    {
        std::ofstream outf(outputHeaderFileName.c_str());
        if(!outf.good())
        {
            std::cerr << "Failed to open output header file "  << outputHeaderFileName << std::endl;
            return 1;
        }

        CppSchemaCodeEmitter(parsedSchema, outf).processSchema();
    }

    return 0;
}
