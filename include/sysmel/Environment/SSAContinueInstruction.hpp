#ifndef SYSMEL_ENVIRONMENT_SSA_CONTINUE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_CONTINUE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAContinueInstruction : public SubtypeOf<SSAInstruction, SSAContinueInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAContinueInstruction";

    virtual bool isSSAContinueInstruction() const override;
    virtual bool isTerminatorInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CONTINUE_INSTRUCTION_HPP