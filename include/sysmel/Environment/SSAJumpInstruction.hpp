#ifndef SYSMEL_ENVIRONMENT_SSA_JUMP_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_JUMP_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAJumpInstruction : public SubtypeOf<SSAInstruction, SSAJumpInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAJumpInstruction";

    virtual bool isSSAJumpInstruction() const override;
    virtual bool isTerminatorInstruction() const override;
    virtual TypePtr getValueType() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

protected:
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_JUMP_INSTRUCTION_HPP