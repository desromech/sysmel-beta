#ifndef SYSMEL_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP
#pragma once

#include "ASTCastNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for accessing a variable.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTImplicitCastNode : public SubtypeOf<ASTCastNode, ASTImplicitCastNode>
{
public:
    static constexpr char const __typeName__[] = "ASTImplicitCastNode";

    virtual bool isASTImplicitCastNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP