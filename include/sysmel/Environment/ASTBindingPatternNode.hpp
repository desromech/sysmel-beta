#ifndef SYSMEL_ENVIRONMENT_AST_BINDING_PATTERN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_BINDING_PATTERN_NODE_HPP
#pragma once

#include "ASTPatternNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTBindingPatternNode : public SubtypeOf<ASTPatternNode, ASTBindingPatternNode>
{
public:
    static constexpr char const __typeName__[] = "ASTBindingPatternNode";

    virtual bool isASTBindingPatternNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual bool isAlwaysMatchingPattern() const override;
    virtual bool isNeverMatchingPattern() const override;
    
    virtual ASTNodePtr optimizePatternNodeForExpectedTypeWith(const TypePtr &type, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr expandPatternNodeForExpectedTypeWith(const TypePtr &type, const ASTNodePtr &patternValueNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    ASTNodePtr identifier;
    ASTNodePtr expectedType;
    ASTNodePtr expectedValue;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_BINDING_VALUE_PATTERN_NODE_HPP