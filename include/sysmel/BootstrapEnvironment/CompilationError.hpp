#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_HPP
#pragma once

#include "Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(CompilationError);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompilationErrors);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ParseError);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SemanticError);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeEvaluationError);

/**
 * I am a generic compilation error.
 */
class CompilationError : public SubtypeOf<Error, CompilationError>
{
public:
    static constexpr char const __typeName__[] = "CompilationError";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_HPP
