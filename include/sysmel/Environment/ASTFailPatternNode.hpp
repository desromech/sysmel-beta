#ifndef SYSMEL_ENVIRONMENT_AST_FAIL_PATTERN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_FAIL_PATTERN_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I indicate failure on pattern matching evaluation.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTFailPatternNode : public SubtypeOf<ASTNode, ASTFailPatternNode>
{
public:
    static constexpr char const __typeName__[] = "ASTFailPatternNode";

    virtual bool isASTFailPatternNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_FAIL_PATTERN_NODE_HPP
