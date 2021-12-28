#ifndef SYSMEL_ENVIRONMENT_AST_EXPLICIT_CAST_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_EXPLICIT_CAST_NODE_HPP
#pragma once

#include "ASTCastNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for accessing a variable.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTExplicitCastNode : public SubtypeOf<ASTCastNode, ASTExplicitCastNode>
{
public:
    static constexpr char const __typeName__[] = "ASTExplicitCastNode";

    virtual bool isASTExplicitCastNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_EXPLICIT_CAST_NODE_HPP