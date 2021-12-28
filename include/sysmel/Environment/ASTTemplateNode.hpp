#ifndef SYSMEL_ENVIRONMENT_AST_TEMPLATE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_TEMPLATE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"
#include "DllLinkageMode.hpp"
#include "ExternalLanguageMode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for defining functions.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTTemplateNode : public SubtypeOf<ASTProgramEntityNode, ASTTemplateNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTemplateNode";

    virtual bool isASTTemplateNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr name;
    ASTNodePtrList arguments;
    ASTNodePtr body;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
    DllLinkageMode dllLinkageMode = DllLinkageMode::None;
    ExternalLanguageMode externalLanguageMode = ExternalLanguageMode::None;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_TEMPLATE_NODE_HPP