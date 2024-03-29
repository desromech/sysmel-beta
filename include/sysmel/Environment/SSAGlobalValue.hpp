#ifndef SYSMEL_ENVIRONMENT_SSA_GLOBAL_VALUE_HPP
#define SYSMEL_ENVIRONMENT_SSA_GLOBAL_VALUE_HPP
#pragma once

#include "SSAConstantValue.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAGlobalValue : public SubtypeOf<SSAConstantValue, SSAGlobalValue>
{
public:
    static constexpr char const __typeName__[] = "SSAGlobalValue";

    virtual bool isSSAGlobalValue() const override;
    virtual AnyValuePtr getName() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_GLOBAL_VALUE_HPP