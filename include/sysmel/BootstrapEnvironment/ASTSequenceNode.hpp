#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEQUENCE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEQUENCE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSequenceNode : public SubtypeOf<ASTNode, ASTSequenceNode>
{
public:
    static constexpr char const __typeName__[] = "ASTSequenceNode";

    virtual bool isASTSequenceNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtrList pragmas;
    ASTNodePtrList expressions;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEQUENCE_NODE_HPP