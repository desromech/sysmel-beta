#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PROGRAM_ENTITY_EXTENSION_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PROGRAM_ENTITY_EXTENSION_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTProgramEntityExtensionNode : public SubtypeOf<ASTProgramEntityNode, ASTProgramEntityExtensionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTProgramEntityExtensionNode";

    virtual bool isASTProgramEntityExtensionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr programEntity;
    ASTNodePtr body;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_NODE_HPP