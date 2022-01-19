#ifndef SYSMEL_ENVIRONMENT_AST_SEQUENCE_PATTERN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_SEQUENCE_PATTERN_NODE_HPP
#pragma once

#include "ASTPatternNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTSequencePatternNode : public SubtypeOf<ASTPatternNode, ASTSequencePatternNode>
{
public:
    static constexpr char const __typeName__[] = "ASTSequencePatternNode";

    virtual bool isASTSequencePatternNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr expectedSequenceType;
    ASTNodePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SEQUENCE_PATTERN_NODE_HPP