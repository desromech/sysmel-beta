#ifndef SYSMEL_ENVIRONMENT_AST_FUNCTIONAL_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_FUNCTIONAL_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "ProgramEntityVisibility.hpp"
#include "MethodFlags.hpp"

namespace Sysmel
{
namespace Environment
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
    MethodFlags methodFlags = MethodFlags::None;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_FUNCTIONAL_NODE_HPP