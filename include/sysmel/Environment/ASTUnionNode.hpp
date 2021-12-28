#ifndef SYSMEL_ENVIRONMENT_AST_UNION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_UNION_NODE_HPP
#pragma once

#include "ASTTypeNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTUnionNode : public SubtypeOf<ASTTypeNode, ASTUnionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTUnionNode";

    virtual bool isASTUnionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_UNION_NODE_HPP