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

    virtual bool isAlwaysMatchingPattern() const;
    virtual bool isNeverMatchingPattern() const;
    
    virtual ASTNodePtr optimizePatternNodeForExpectedTypeWith(const TypePtr &type, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr expandPatternNodeForExpectedTypeWith(const TypePtr &type, const ASTNodePtr &patternValueNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    ASTNodePtr expectedSequenceType;
    ASTNodePtrList elements;
    TypePtrList elementTypes;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SEQUENCE_PATTERN_NODE_HPP