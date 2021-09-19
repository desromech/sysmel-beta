#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_CLOSURE_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_CLOSURE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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
class ASTClosureNode : public SubtypeOf<ASTNode, ASTClosureNode>
{
public:
    static constexpr char const __typeName__[] = "ASTClosureNode";

    virtual bool isASTClosureNode() const override;
    virtual AnyValuePtr encodeAsSExpression() const override;

    ASTClosureNodeKind kind = ASTClosureNodeKind::Function;
    ASTNodePtrList arguments;
    ASTNodePtr returnType;
    ASTNodePtr body;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_CLOSURE_NODE_HPP