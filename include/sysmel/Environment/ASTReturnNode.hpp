#ifndef SYSMEL_ENVIRONMENT_AST_RETURN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_RETURN_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTReturnNode : public SubtypeOf<ASTNode, ASTReturnNode>
{
public:
    static constexpr char const __typeName__[] = "ASTReturnNode";

    virtual bool isASTReturnNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr expression;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_RETURN_NODE_HPP