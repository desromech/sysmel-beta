#ifndef SYSMEL_ENVIRONMENT_SSA_TRAP_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_TRAP_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"
#include "TrapReason.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSATrapInstruction : public SubtypeOf<SSAInstruction, SSATrapInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSATrapInstruction";

    virtual bool isSSATrapInstruction() const override;
    virtual bool isTerminatorInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;

    void setReason(TrapReason newReason);
    TrapReason getReason() const;

private:
    TrapReason reason = TrapReason::Generic;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_TRAP_INSTRUCTION_HPP