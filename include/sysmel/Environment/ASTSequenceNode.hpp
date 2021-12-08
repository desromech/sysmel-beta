#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEQUENCE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEQUENCE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSequenceNode : public SubtypeOf<ASTNode, ASTSequenceNode>
{
public:
    static constexpr char const __typeName__[] = "ASTSequenceNode";

    virtual bool isASTSequenceNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtrList pragmas;
    ASTNodePtrList expressions;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEQUENCE_NODE_HPP