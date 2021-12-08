#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_EVALUATION_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_EVALUATION_ERROR_HPP
#pragma once

#include "CompilationError.hpp"
#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a collection of compilation errors.
 */
class CompileTimeEvaluationError : public SubtypeOf<CompilationError, CompileTimeEvaluationError>
{
public:
    static constexpr char const __typeName__[] = "CompileTimeEvaluationError";

    virtual std::string getDescription() const override;

    ASTCompileTimeEvaluationErrorNodePtr compileTimeEvaluationErrorNode;
    ErrorPtr caughtError;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_EVALUATION_ERROR_HPP
