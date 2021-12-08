#ifndef SYSMEL_ENVIRONMENT_SSA_CONSTANT_VALUE_HPP
#define SYSMEL_ENVIRONMENT_SSA_CONSTANT_VALUE_HPP
#pragma once

#include "SSAValue.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAConstantValue : public SubtypeOf<SSAValue, SSAConstantValue>
{
public:
    static constexpr char const __typeName__[] = "SSAConstantValue";

    virtual bool isSSAConstantValue() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CONSTANT_VALUE_HPP