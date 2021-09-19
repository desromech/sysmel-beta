#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IDENTIFIER_REFERENCE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IDENTIFIER_REFERENCE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node references a specific identifier.
 */
class ASTIdentifierReferenceNode : public SubtypeOf<ASTNode, ASTIdentifierReferenceNode>
{
public:
    static constexpr char const __typeName__[] = "ASTIdentifierReferenceNode";

    virtual bool isASTIdentifierReferenceNode() const override;
    virtual SExpression asSExpression() const override;

    AnyValuePtr identifier;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IDENTIFIER_REFERENCE_NODE_HPP