#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_DOWNCAST_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_DOWNCAST_INSTRUCTION_HPP
#pragma once

#include "SSACastInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSADowncastInstruction : public SubtypeOf<SSACastInstruction, SSADowncastInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSADowncastInstruction";

    virtual bool isSSADowncastInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_DOWNCAST_INSTRUCTION_HPP