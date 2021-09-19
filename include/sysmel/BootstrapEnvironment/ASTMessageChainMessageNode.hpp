#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MESSAGE_CHAIN_MESSAGE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MESSAGE_CHAIN_MESSAGE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I represent single message in a message chain.
 */
class ASTMessageChainMessageNode : public SubtypeOf<ASTNode, ASTMessageChainMessageNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMessageChainMessageNode";

    virtual bool isASTMessageChainMessageNode() const override;
    virtual AnyValuePtr encodeAsSExpression() const override;

    ASTNodePtr selector;
    ASTNodePtrList arguments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MESSAGE_CHAIN_MESSAGE_NODE_HPP