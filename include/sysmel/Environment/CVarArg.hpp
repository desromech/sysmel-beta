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
class SYSMEL_COMPILER_LIB_EXPORT CVarArg : public SubtypeOf<PrimitiveType, CVarArg>
{
public:
    static constexpr char const __typeName__[] = "CVarArg";
    static constexpr char const __sysmelTypeName__[] = "CVarArg";
    
    static AnyValuePtrList __constructors__();

    static CVarArgPtr make(const AnyValuePtr &value);

    virtual bool isCVarArg() const override;

    AnyValuePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CVAR_ARG_HPP