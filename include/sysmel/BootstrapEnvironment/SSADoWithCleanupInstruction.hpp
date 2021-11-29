#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_DO_WHILE_CLEANUP_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_DO_WHILE_CLEANUP_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSADoWithCleanupInstruction : public SubtypeOf<SSAInstruction, SSADoWithCleanupInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSADoWithCleanupInstruction";

    virtual bool isSSADoWithCleanupInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_DO_WHILE_CLEANUP_STATEMENT_INSTRUCTION_HPP