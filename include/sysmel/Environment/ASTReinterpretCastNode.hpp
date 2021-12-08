#ifndef SYSMEL_ENVIRONMENT_AST_REINTERPRET_CAST_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_REINTERPRET_CAST_NODE_HPP
#pragma once

#include "ASTCastNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for accessing a variable.
 */
class ASTReinterpretCastNode : public SubtypeOf<ASTCastNode, ASTReinterpretCastNode>
{
public:
    static constexpr char const __typeName__[] = "ASTReinterpretCastNode";

    virtual bool isASTReinterpretCastNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_IMPLICIT_CAST_NODE_HPP