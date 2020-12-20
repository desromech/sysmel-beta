#include "sysmel/BaseCompiler/SObjectSchema.hpp"
#include <iostream>
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


    void beginType(SchemaType &entity)
    {
        out << "struct " << entity.name;
        if(!entity.superTypeName.empty())
            out << " : " << entity.superTypeName;
        out << "\n";
        out << "{\n";
    }

    void endType(SchemaType &)
    {
        out << "};\n";
        out << "\n";
    }

    virtual std::any visitType(SchemaType &entity) override
    {
        setActiveNamespace(entity.fullNamespace);

        beginType(entity);
        endType(entity);

        return std::any();
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
        for(auto &entity : schema->entities)
        {
            visitEntity(*entity);
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
    std::string outputHeader = argv[2];

    SchemaPtr parsedSchema;
    try
    {
        parsedSchema = parseSchemaFromFileNamed(inputSSchemaFileName);
        if(!parsedSchema)
            return 1;
    }
    catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    CppSchemaCodeEmitter(parsedSchema, std::cout).processSchema();
    return 0;
}
