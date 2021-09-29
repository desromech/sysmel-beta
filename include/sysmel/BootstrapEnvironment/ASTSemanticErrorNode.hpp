#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP
#pragma once

#include "ASTErrorNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSemanticErrorNode : public SubtypeOf<ASTErrorNode, ASTSemanticErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTSemanticErrorNode";

    virtual bool isASTSemanticErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual CompilationErrorPtr asCompilationError() override;

    std::string errorMessage;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP