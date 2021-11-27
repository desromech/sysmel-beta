#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TEMPLATE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TEMPLATE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for defining functions.
 */
class ASTTemplateNode : public SubtypeOf<ASTProgramEntityNode, ASTTemplateNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTemplateNode";

    virtual bool isASTTemplateNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr name;
    ASTNodePtrList arguments;
    ASTNodePtr body;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TEMPLATE_NODE_HPP