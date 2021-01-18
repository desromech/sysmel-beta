#ifndef SYSMEL_COMPILER_BOOTSTRAP_INTERPRETER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_INTERPRETER_HPP
#pragma once

#include "AST.hpp"
#include "sysmel/ObjectModel/ObjectEnvironment.hpp"
#include "sysmel/ObjectModel/SObjectSchema.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{
class BootstrapEvaluationEnvironment;
typedef std::shared_ptr<BootstrapEvaluationEnvironment> BootstrapEvaluationEnvironmentPtr;

class BootstrapEvaluationEnvironment
{
public:
    virtual ~BootstrapEvaluationEnvironment() {};

    BootstrapEvaluationEnvironment() {}
    BootstrapEvaluationEnvironment(const BootstrapEvaluationEnvironmentPtr &cparent)
        : parent(cparent) {}

    virtual std::optional<ObjectModel::ObjectPtr> lookSymbol(const std::string &)
    {
        return std::nullopt;
    }

    virtual std::optional<ObjectModel::ObjectPtr> lookSymbolRecursively(const std::string &symbol)
    {
        auto found = lookSymbol(symbol);
        if(found.has_value())
            return found;
        if(parent)
            return parent->lookSymbolRecursively(symbol);
        return std::nullopt;
    }

private:
    BootstrapEvaluationEnvironmentPtr parent;
};

class BootstrapLexicalScopeEnvironment : public BootstrapEvaluationEnvironment
{
public:
    BootstrapLexicalScopeEnvironment(const BootstrapEvaluationEnvironmentPtr &cparent)
        : BootstrapEvaluationEnvironment(cparent) {}

    void setSymbol(const std::string &symbol, const ObjectModel::ObjectPtr &symbolValue)
    {
        symbolTable[symbol] = symbolValue;
    }

    virtual std::optional<ObjectModel::ObjectPtr> lookSymbol(const std::string &symbol) override
    {
        auto it = symbolTable.find(symbol);
        if(it != symbolTable.end())
            return it->second;
        return std::nullopt;
    }

private:
    std::unordered_map<std::string, ObjectModel::ObjectPtr> symbolTable;
};

class BootstrapSchemaTypesEnvironment : public BootstrapEvaluationEnvironment
{
public:
    BootstrapSchemaTypesEnvironment(const BootstrapEvaluationEnvironmentPtr &cparent, const ObjectModel::SchemaPtr &cschema)
        : BootstrapEvaluationEnvironment(cparent), schema(cschema) {}


    virtual std::optional<ObjectModel::ObjectPtr> lookSymbol(const std::string &symbol) override
    {
        auto type = schema->getTypeDefinitionNamed(symbol);
        if(type)
            return type;
        return std::nullopt;
    }

private:
    ObjectModel::SchemaPtr schema;
};

BootstrapEvaluationEnvironmentPtr createDefaultBootstrapEvaluationEnvironmentFor(const ObjectModel::ObjectEnvironmentPtr &objectEnvironment);
ObjectModel::ObjectPtr evaluateBootstrapScriptAST(const ASTNodePtr &ast, const BootstrapEvaluationEnvironmentPtr &evaluationEnvironment);

} // End of namespace Sysmel.
} // End of namespace Compiler.
} // End of namespace SysmelMoebius.

#endif // SYSMEL_COMPILER_BOOTSTRAP_INTERPRETER_HPP
