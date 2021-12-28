#ifndef SYSMEL_ENVIRONMENT_AST_PROGRAM_ENTITY_EXTENSION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_PROGRAM_ENTITY_EXTENSION_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTProgramEntityExtensionNode : public SubtypeOf<ASTProgramEntityNode, ASTProgramEntityExtensionNode>
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_TYPE_NODE_HPP