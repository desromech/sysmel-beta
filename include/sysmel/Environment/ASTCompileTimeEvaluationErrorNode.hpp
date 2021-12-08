#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATION_ERROR_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATION_ERROR_NODE_HPP
#pragma once

#include "ASTErrorNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Error);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTCompileTimeEvaluationErrorNode : public SubtypeOf<ASTErrorNode, ASTCompileTimeEvaluationErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCompileTimeEvaluationErrorNode";

    virtual bool isASTCompileTimeEvaluationErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual CompilationErrorPtr asCompilationError() override;

    std::string errorMessage;
    ErrorPtr caughtError;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATION_ERROR_NODE_HPP