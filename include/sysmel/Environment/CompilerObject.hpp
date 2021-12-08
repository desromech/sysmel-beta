#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILER_OBJECT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILER_OBJECT_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class CompilerObject : public SubtypeOf<AnyValue, CompilerObject>
{
public:
    static constexpr char const __typeName__[] = "CompilerObject";

    virtual bool isCompilerObject() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP