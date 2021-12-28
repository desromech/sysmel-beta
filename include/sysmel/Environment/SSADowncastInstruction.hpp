#ifndef SYSMEL_ENVIRONMENT_SSA_DOWNCAST_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_DOWNCAST_INSTRUCTION_HPP
#pragma once

#include "SSACastInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSADowncastInstruction : public SubtypeOf<SSACastInstruction, SSADowncastInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSADowncastInstruction";

    virtual bool isSSADowncastInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_DOWNCAST_INSTRUCTION_HPP