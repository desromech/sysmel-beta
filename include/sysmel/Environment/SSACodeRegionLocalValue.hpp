#ifndef SYSMEL_ENVIRONMENT_SSA_CODE_REGION_LOCAL_VALUE_HPP
#define SYSMEL_ENVIRONMENT_SSA_CODE_REGION_LOCAL_VALUE_HPP
#pragma once

#include "SSAValue.hpp"

namespace Sysmel
{
namespace Environment
{

struct SSACodeRegionLocalValueEnumerationState
{
    uint32_t localValueCount = 0;
    uint32_t basicBlockCount = 0;
};

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACodeRegionLocalValue : public SubtypeOf<SSAValue, SSACodeRegionLocalValue>
{
public:
    static constexpr char const __typeName__[] = "SSACodeRegionLocalValue";

    virtual bool isSSACodeRegionLocalValue() const override;

    virtual SExpression asSExpression() const override;

    virtual void enumerateLocalValues(SSACodeRegionLocalValueEnumerationState &state);
protected:
    uint32_t localValueIndex;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CODE_REGION_LOCAL_VALUE_HPP