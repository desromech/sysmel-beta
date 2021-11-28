#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_ENUM_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_ENUM_NODE_HPP
#pragma once

#include "ASTTypeNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTEnumNode : public SubtypeOf<ASTTypeNode, ASTEnumNode>
{
public:
    static constexpr char const __typeName__[] = "ASTEnumNode";

    virtual bool isASTEnumNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr valueType;
    ASTNodePtr values;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_ENUM_NODE_HPP