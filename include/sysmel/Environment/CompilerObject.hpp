#ifndef SYSMEL_ENVIRONMENT_COMPILER_OBJECT_HPP
#define SYSMEL_ENVIRONMENT_COMPILER_OBJECT_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompilerObject);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT CompilerObject : public SubtypeOf<AnyValue, CompilerObject>
{
public:
    static constexpr char const __typeName__[] = "CompilerObject";

    virtual bool isCompilerObject() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PROGRAM_ENTITY_HPP