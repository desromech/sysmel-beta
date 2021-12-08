#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IDENTIFIER_REFERENCE_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IDENTIFIER_REFERENCE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node references a specific identifier.
 */
class ASTIdentifierReferenceNode : public SubtypeOf<ASTNode, ASTIdentifierReferenceNode>
{
public:
    static constexpr char const __typeName__[] = "ASTIdentifierReferenceNode";

    virtual bool isASTIdentifierReferenceNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual ASTNodePtr parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    AnyValuePtr identifier;
    AnyValuePtr binding;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_IDENTIFIER_REFERENCE_NODE_HPP