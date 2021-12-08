#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_TUPLE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_TUPLE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class ASTMakeTupleNode : public SubtypeOf<ASTNode, ASTMakeTupleNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeTupleNode";

    virtual bool isASTMakeTupleNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_TUPLE_NODE_HPP