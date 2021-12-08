#ifndef SYSMEL_ENVIRONMENT_AST_CAST_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_CAST_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_CAST_NODE_HPP