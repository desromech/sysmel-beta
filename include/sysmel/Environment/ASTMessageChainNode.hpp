#ifndef SYSMEL_ENVIRONMENT_AST_MESSAGE_CHAIN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_MESSAGE_CHAIN_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am represent a message chain AST node.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMessageChainNode : public SubtypeOf<ASTNode, ASTMessageChainNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMessageChainNode";

    virtual bool isASTMessageChainNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr receiver;
    ASTNodePtrList messages;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_MESSAGE_CHAIN_NODE_HPP