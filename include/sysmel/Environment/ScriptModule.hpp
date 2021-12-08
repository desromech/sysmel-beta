#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SCRIPT_MODULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SCRIPT_MODULE_HPP
#pragma once

#include "ProgramModule.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ScriptModule);

/**
 * I am a particular module in the system.
 */
class ScriptModule : public SubtypeOf<ProgramModule, ScriptModule>
{
public:
    static constexpr char const __typeName__[] = "ScriptModule";

    static ScriptModulePtr create();

    virtual bool isScriptModule() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SCRIPT_MODULE_HPP