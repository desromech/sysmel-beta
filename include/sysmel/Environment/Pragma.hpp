#ifndef SYSMEL_ENVIRONMENT_PRAGMA_HPP
#define SYSMEL_ENVIRONMENT_PRAGMA_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRAGMA_HPP
