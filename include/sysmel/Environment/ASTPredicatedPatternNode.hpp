#ifndef SYSMEL_ENVIRONMENT_AST_PREDICATED_PATTERN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_PREDICATED_PATTERN_NODE_HPP
#pragma once

#include "ASTPatternNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTPredicatedPatternNode : public SubtypeOf<ASTPatternNode, ASTPredicatedPatternNode>
{
public:
    static constexpr char const __typeName__[] = "ASTPredicatedPatternNode";

    virtual bool isASTPredicatedPatternNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr expectedPattern;
    ASTNodePtr predicate;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_PREDICATED_PATTERN_NODE_HPP