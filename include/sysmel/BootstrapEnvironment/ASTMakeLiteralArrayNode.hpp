#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_LITERAL_ARRAY_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_LITERAL_ARRAY_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class ASTMakeLiteralArrayNode : public SubtypeOf<ASTNode, ASTMakeLiteralArrayNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeLiteralArrayNode";

    virtual bool isASTMakeLiteralArrayNode() const override;
    virtual SExpression asSExpression() const override;

    ASTNodePtrList elements;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_LITERAL_ARRAY_NODE_HPP