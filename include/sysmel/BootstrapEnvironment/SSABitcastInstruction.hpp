#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BITCAST_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BITCAST_INSTRUCTION_HPP
#pragma once

#include "SSACastInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSABitcastInstruction : public SubtypeOf<SSACastInstruction, SSABitcastInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSABitcastInstruction";

    virtual bool isSSABitcastInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BITCAST_INSTRUCTION_HPP