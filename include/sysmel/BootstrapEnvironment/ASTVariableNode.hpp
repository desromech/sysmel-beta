#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_VARIABLE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_VARIABLE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a variable.
 */
class ASTVariableNode : public SubtypeOf<ASTProgramEntityNode, ASTVariableNode>
{
public:
    static constexpr char const __typeName__[] = "ASTVariableNode";

    virtual bool isASTVariableNode() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr name;
    ASTNodePtr type;
    ASTNodePtr alignment;
    ASTNodePtr initialValue;

    TypeInferenceMode typeInferenceMode = TypeInferenceMode::Value;
    bool isMutable = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_VARIABLE_NODE_HPP