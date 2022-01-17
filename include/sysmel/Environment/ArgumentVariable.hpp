#ifndef SYSMEL_ENVIRONMENT_ARGUMENT_VARIABLE_HPP
#define SYSMEL_ENVIRONMENT_ARGUMENT_VARIABLE_HPP
#pragma once

#include "FunctionVariable.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);

/**
 * I am a variable program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ArgumentVariable : public SubtypeOf<FunctionVariable, ArgumentVariable>
{
public:
    static constexpr char const __typeName__[] = "ArgumentVariable";

    virtual bool isArgumentVariable() const override;

    void setType(const TypePtr &type);
    void setName(const AnyValuePtr &newName);
    void setArgumentDeclarationNode(const ASTArgumentDefinitionNodePtr &node);
    void setArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node);

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;
    
    bool isImplicit = false;
    bool isReceiver = false;
    uint32_t argumentIndex = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_ARGUMENT_VARIABLE_HPP