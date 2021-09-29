#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRAGMA_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRAGMA_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Pragma);

/**
 * I am the base interface for any program metamodel entity
 */
class Pragma : public SubtypeOf<CompilerObject, Pragma>
{
public:
    static constexpr char const __typeName__[] = "Pragma";

    virtual bool isPragma() const override;

    AnyValuePtr selector;
    AnyValuePtrList arguments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRAGMA_HPP
