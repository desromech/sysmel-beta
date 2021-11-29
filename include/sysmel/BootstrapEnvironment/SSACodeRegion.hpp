#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_HPP
#pragma once

#include "SSAValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACodeRegion : public SubtypeOf<SSAValue, SSACodeRegion>
{
public:
    static constexpr char const __typeName__[] = "SSACodeRegion";

    virtual bool isSSACodeRegion() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_HPP