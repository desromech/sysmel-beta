#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_QUASI_QUOTE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_QUASI_QUOTE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTQuasiQuoteNode : public SubtypeOf<ASTNode, ASTQuasiQuoteNode>
{
public:
    static constexpr char const __typeName__[] = "ASTQuasiQuoteNode";

    virtual bool isASTQuasiQuoteNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr expression;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_QUASI_QUOTE_NODE_HPP