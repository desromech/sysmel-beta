#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERRORS_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERRORS_HPP
#pragma once

#include "CompilationError.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a collection of compilation errors.
 */
class CompilationErrors : public SubtypeOf<CompilationError, CompilationErrors>
{
public:
    static constexpr char const __typeName__[] = "CompilationErrors";

    virtual std::string getDescription() const override;
    
    CompilationErrorPtrList errors;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_HPP
