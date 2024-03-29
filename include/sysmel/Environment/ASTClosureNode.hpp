#ifndef SYSMEL_ENVIRONMENT_AST_CLOSURE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_CLOSURE_NODE_HPP
#pragma once

#include "ASTProgramEntityNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I specify the kind of a closure node.
 */
enum class ASTClosureNodeKind : uint8_t
{
    Function = 0,
    Block,
};

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTClosureNode : public SubtypeOf<ASTProgramEntityNode, ASTClosureNode>
{
public:
    static constexpr char const __typeName__[] = "ASTClosureNode";

    virtual bool isASTClosureNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    
    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTClosureNodeKind kind = ASTClosureNodeKind::Function;
    ASTNodePtrList arguments;
    ASTNodePtr resultType;
    ASTNodePtr body;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_CLOSURE_NODE_HPP