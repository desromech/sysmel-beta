#ifndef SYSMEL_ENVIRONMENT_AST_LOCAL_IMMUTABLE_ACCESS_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_LOCAL_IMMUTABLE_ACCESS_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Variable);

/**
 * I am a node that is used for accessing a variable.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTLocalImmutableAccessNode : public SubtypeOf<ASTNode, ASTLocalImmutableAccessNode>
{
public:
    static constexpr char const __typeName__[] = "ASTLocalImmutableAccessNode";

    virtual bool isASTLocalImmutableAccessNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    AnyValuePtr bindingName;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_LOCAL_IMMUTABLE_ACCESS_NODE_HPP