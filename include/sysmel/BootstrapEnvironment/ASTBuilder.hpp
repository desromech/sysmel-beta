#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_BUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_BUILDER_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageChainNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageChainMessageNode);
/**
 * I am an utility for building AST nodes.
 */
class ASTBuilder : public SubtypeOf<CompilerObject, ASTBuilder>
{
public:
    static constexpr char const __typeName__[] = "ASTBuilder";

    virtual bool isASTBuilder() const override;

    ASTLiteralValueNodePtr literal(const AnyValuePtr &value);
    ASTLiteralValueNodePtr literalSymbol(const std::string &symbol);
    ASTMessageChainNodePtr messageChain(const ASTNodePtr &receiver, const ASTNodePtrList &messages);
    ASTMessageChainMessageNodePtr messageChainMessage(const ASTNodePtr &selector, const ASTNodePtrList &arguments);
    ASTMessageSendNodePtr sendToWithArguments(const ASTNodePtr &selector, const ASTNodePtr &receiver, const ASTNodePtrList &arguments);

    ASTSourcePositionPtr sourcePosition;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_BUILDER_HPP