#ifndef SYSMEL_ENVIRONMENT_AST_MESSAGE_SEND_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_MESSAGE_SEND_NODE_HPP
#pragma once

#include "ASTNode.hpp"
#include "Type.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalType)

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMessageSendNode : public SubtypeOf<ASTNode, ASTMessageSendNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMessageSendNode";

    virtual bool isASTMessageSendNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    virtual ASTNodePtr parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr parseAsPatternNode() override;

    bool isPureCompileTimeLiteralMessage() const;

    MessageSendExpansionLevel expansionLevel = MessageSendExpansionLevel::UnexpandedMacros;
    ASTNodePtr selector;
    ASTNodePtr receiver;
    ASTNodePtrList arguments;

    AnyValuePtr analyzedBoundMessage;
    FunctionalTypePtr calledMessageType;
    bool analyzedBoundMessageIsDirect = false;
    bool isPureMessageSend = false; // TODO: Replace this with a pure flag.
    bool useVirtualTable = false;
    uint32_t virtualTableSlotIndex = 0;
    uint32_t virtualTableEntrySlotIndex = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_MESSAGE_SEND_NODE_HPP