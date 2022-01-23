#ifndef SYSMEL_ENVIRONMENT_AST_EVALUATE_PATTERN_WITH_VALUE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_EVALUATE_PATTERN_WITH_VALUE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope);

/**
 * I am node that is used for evaluating a pattern matching pattern with a specific value.
 * I am typically used for implementing destructuring binding and pattern matching.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTEvaluatePatternWithValueNode : public SubtypeOf<ASTNode, ASTEvaluatePatternWithValueNode>
{
public:
    static constexpr char const __typeName__[] = "ASTEvaluatePatternWithValueNode";

    virtual bool isASTEvaluatePatternWithValueNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    
    bool introduceNewLexicalScope = false;
    LexicalScopePtr patternScope;

    ASTNodePtr patternNode;
    ASTNodePtr valueNode;
    ASTNodePtr patternEvaluationNode;
    ASTNodePtr successAction;
    ASTNodePtr failureAction;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_EVALUATE_PATTERN_WITH_VALUE_NODE_HPP
