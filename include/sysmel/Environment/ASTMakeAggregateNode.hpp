#ifndef SYSMEL_ENVIRONMENT_AST_MAKE_AGGREGATE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_MAKE_AGGREGATE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMakeAggregateNode : public SubtypeOf<ASTNode, ASTMakeAggregateNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeAggregateNode";

    virtual bool isASTMakeAggregateNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtrList elements;
    ASTNodePtr aggregateType;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_MAKE_AGGREGATE_NODE_HPP