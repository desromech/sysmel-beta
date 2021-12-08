#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_VALUE_AS_VOID_TYPE_CONVERSION_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_VALUE_AS_VOID_TYPE_CONVERSION_NODE_HPP
#pragma once

#include "ASTTypeConversionNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTValueAsVoidTypeConversionNode : public SubtypeOf<ASTTypeConversionNode, ASTValueAsVoidTypeConversionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTValueAsVoidTypeConversionNode";

    virtual bool isASTValueAsVoidTypeConversionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_VALUE_AS_VOID_TYPE_CONVERSION_NODE_HPP