#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CAST_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CAST_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for accessing a variable.
 */
class ASTCastNode : public SubtypeOf<ASTNode, ASTCastNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCastNode";

    virtual bool isASTCastNode() const override;
    
    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr expression;
    ASTNodePtr targetType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_CAST_NODE_HPP