#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_MESSAGE_SEND_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_MESSAGE_SEND_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * The diferent levels for a message send expansion.
 */
enum class ASTMessageSendExpansionLevel : uint8_t
{
    UnexpandedMacros = 0,
    ExpandedMacros
};

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTMessageSendNode : public SubtypeOf<ASTNode, ASTMessageSendNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMessageSendNode";

    virtual bool isASTMessageSendNode() const override;

    ASTMessageSendExpansionLevel expansionLevel = ASTMessageSendExpansionLevel::UnexpandedMacros;
    ASTNodePtr selector;
    ASTNodePtr receiver;
    ASTNodePtrList arguments;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_MESSAGE_SEND_NODE_HPP