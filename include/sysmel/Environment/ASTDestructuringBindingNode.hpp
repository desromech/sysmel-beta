#ifndef SYSMEL_ENVIRONMENT_AST_DESTRUCTURING_BINDING_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_DESTRUCTURING_BINDING_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a destructuring binding binding. I use pattern matching as convenience form for introducing new local variable bindings.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTDestructuringBindingNode : public SubtypeOf<ASTNode, ASTDestructuringBindingNode>
{
public:
    static constexpr char const __typeName__[] = "ASTDestructuringBindingNode";

    virtual bool isASTDestructuringBindingNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    
    ASTNodePtr patternNode;
    ASTNodePtr valueNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_DESTRUCTURING_BINDING_NODE_HPP
