#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FUNCTIONAL_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FUNCTIONAL_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"
#include "MethodDispatchMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a functional object.
 */
class ASTFunctionalNode : public SubtypeOf<ASTProgramEntityNode, ASTFunctionalNode>
{
public:
    static constexpr char const __typeName__[] = "ASTFunctionalNode";

    virtual bool isASTFunctionalNode() const override;
    
    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    bool isRegisteredInLexicalScope() const;
    
    ASTNodePtr name;
    ASTNodePtrList arguments;
    ASTNodePtr resultType;
    ASTNodePtr body;

    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
    MethodDispatchMode dispatchMode = MethodDispatchMode::Default;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_FUNCTIONAL_NODE_HPP