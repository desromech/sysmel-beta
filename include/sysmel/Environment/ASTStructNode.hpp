#ifndef SYSMEL_ENVIRONMENT_AST_STRUCT_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_STRUCT_NODE_HPP
#pragma once

#include "ASTTypeNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTStructNode : public SubtypeOf<ASTTypeNode, ASTStructNode>
{
public:
    static constexpr char const __typeName__[] = "ASTStructNode";

    virtual bool isASTStructNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    bool isPacked = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_STRUCT_NODE_HPP