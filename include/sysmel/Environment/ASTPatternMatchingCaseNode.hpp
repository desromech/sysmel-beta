#ifndef SYSMEL_ENVIRONMENT_AST_PATTERN_MATCHING_CASE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_PATTERN_MATCHING_CASE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a specific case in a pattern matching.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTPatternMatchingCaseNode : public SubtypeOf<ASTNode, ASTPatternMatchingCaseNode>
{
public:
    static constexpr char const __typeName__[] = "ASTPatternMatchingCaseNode";

    virtual bool isASTPatternMatchingCaseNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    
    ASTNodePtr patternNode;
    ASTNodePtr actionNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_PATTERN_MATCHING_CASE_NODE_HPP
