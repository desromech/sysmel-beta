#ifndef SYSMEL_ENVIRONMENT_AST_NAMESPACE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_NAMESPACE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a functional object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTNamespaceNode : public SubtypeOf<ASTProgramEntityNode, ASTNamespaceNode>
{
public:
    static constexpr char const __typeName__[] = "ASTNamespaceNode";

    virtual bool isASTNamespaceNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr name;
    ASTNodePtr body;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_NAMESPACE_NODE_HPP