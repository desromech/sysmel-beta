#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_ERROR_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_ERROR_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompilationError);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTErrorNode : public SubtypeOf<ASTNode, ASTErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTASTErrorNode";

    virtual bool isASTErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual CompilationErrorPtr asCompilationError();

    std::string errorMessage;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP