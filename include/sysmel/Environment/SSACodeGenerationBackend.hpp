#ifndef SYSMEL_ENVIRONMENT_SSA_CODE_GENERATION_BACKEND_HPP
#define SYSMEL_ENVIRONMENT_SSA_CODE_GENERATION_BACKEND_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for a SSA based code generation backend
 */
class SSACodeGenerationBackend : public SubtypeOf<CompilerObject, SSACodeGenerationBackend>
{
public:
    static constexpr char const __typeName__[] = "SSACodeGenerationBackend";

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CODE_GENERATION_BACKEND_HPP