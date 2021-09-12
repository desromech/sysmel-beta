#ifndef SYSMEL_COMPILER_OBJECT_MODEL_COMPILER_OBJECT_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_COMPILER_OBJECT_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_PROGRAM_ENTITY_HPP