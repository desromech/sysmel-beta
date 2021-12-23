#ifndef SYSMEL_ENVIRONMENT_CVAR_ARG_HPP
#define SYSMEL_ENVIRONMENT_CVAR_ARG_HPP
#pragma once

#include "PrimitiveType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CVarArg);


/**
 * I am literal number value.
 */
class CVarArg : public SubtypeOf<PrimitiveType, CVarArg>
{
public:
    static constexpr char const __typeName__[] = "CVarArg";
    static constexpr char const __sysmelTypeName__[] = "CVarArg";

    virtual bool isCVarArg() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CVAR_ARG_HPP