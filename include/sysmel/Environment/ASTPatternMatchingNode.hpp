#ifndef SYSMEL_ENVIRONMENT_AST_PATTERN_MATCHING_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_PATTERN_MATCHING_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I perform pattern matching on a value.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTPatternMatchingNode : public SubtypeOf<ASTNode, ASTPatternMatchingNode>
{
public:
    static constexpr char const __typeName__[] = "ASTPatternMatchingNode";

    virtual bool isASTPatternMatchingNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    
    ASTNodePtr patternNodes;
    ASTNodePtr valueNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_PATTERN_MATCHING_NODE_HPP
