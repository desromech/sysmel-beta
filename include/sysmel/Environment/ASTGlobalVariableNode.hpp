#ifndef SYSMEL_ENVIRONMENT_AST_GLOBAL_VARIABLE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_GLOBAL_VARIABLE_NODE_HPP
#pragma once

#include "ASTVariableNode.hpp"
#include "ProgramEntityVisibility.hpp"
#include "DllLinkageMode.hpp"
#include "ExternalLanguageMode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a program entity.
 */
class ASTGlobalVariableNode : public SubtypeOf<ASTVariableNode, ASTGlobalVariableNode>
{
public:
    static constexpr char const __typeName__[] = "ASTGlobalVariableNode";

    virtual bool isASTGlobalVariableNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
    DllLinkageMode dllLinkageMode = DllLinkageMode::None;
    ExternalLanguageMode externalLanguageMode = ExternalLanguageMode::None;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_GLOBAL_VARIABLE_NODE_HPP