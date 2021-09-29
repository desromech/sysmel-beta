#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_EVALUATION_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_EVALUATION_ERROR_HPP
#pragma once

#include "CompilationError.hpp"
#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_EVALUATION_ERROR_HPP
