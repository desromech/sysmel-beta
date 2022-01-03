#ifndef SYSMEL_ENVIRONMENT_AST_SLOT_ACCESS_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_SLOT_ACCESS_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for accessing a variable.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTSlotAccessNode : public SubtypeOf<ASTNode, ASTSlotAccessNode>
{
public:
    static constexpr char const __typeName__[] = "ASTSlotAccessNode";

    virtual bool isASTSlotAccessNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr aggregate;
    uint64_t slotIndex;
    TypePtr slotReferenceType;
    bool isNotPaddedSlotIndex = true;

    uint64_t typeSelectorSlotIndex = 0;
    TypePtr typeSelectorSlotReferenceType;
    uint64_t expectedTypeSelectorValue = 0;
    bool checkTypeSelectorIndex = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SLOT_ACCESS_NODE_HPP
