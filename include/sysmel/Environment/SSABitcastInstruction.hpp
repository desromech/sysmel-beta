#ifndef SYSMEL_ENVIRONMENT_SSA_BITCAST_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_BITCAST_INSTRUCTION_HPP
#pragma once

#include "SSACastInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSABitcastInstruction : public SubtypeOf<SSACastInstruction, SSABitcastInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSABitcastInstruction";

    virtual bool isSSABitcastInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_BITCAST_INSTRUCTION_HPP