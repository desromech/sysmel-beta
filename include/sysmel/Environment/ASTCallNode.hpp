#ifndef SYSMEL_ENVIRONMENT_AST_CALL_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_CALL_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that applies a generic intrinsic operation for processing its parameters.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTCallNode : public SubtypeOf<ASTNode, ASTCallNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCallNode";

    virtual bool isASTCallNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;
    
    virtual ASTNodePtr parseAsPatternNode() override;

    ASTNodePtr function;
    ASTNodePtrList arguments;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_CALL_NODE_HPP