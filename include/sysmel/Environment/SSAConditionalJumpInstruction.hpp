#ifndef SYSMEL_ENVIRONMENT_SSA_CONDITIONAL_JUMP_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_CONDITIONAL_JUMP_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAConditionalJumpInstruction : public SubtypeOf<SSAInstruction, SSAConditionalJumpInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAConditionalJumpInstruction";

    virtual bool isSSAConditionalJumpInstruction() const override;
    virtual bool isTerminatorInstruction() const override;
    virtual TypePtr getValueType() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_JUMP_INSTRUCTION_HPP