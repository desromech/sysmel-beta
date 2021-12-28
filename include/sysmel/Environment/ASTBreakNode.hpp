#ifndef SYSMEL_ENVIRONMENT_AST_BREAK_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_BREAK_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTBreakNode : public SubtypeOf<ASTNode, ASTBreakNode>
{
public:
    static constexpr char const __typeName__[] = "ASTBreakNode";

    virtual bool isASTBreakNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_BREAK_NODE_HPP