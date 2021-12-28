#ifndef SYSMEL_ENVIRONMENT_SCRIPT_MODULE_HPP
#define SYSMEL_ENVIRONMENT_SCRIPT_MODULE_HPP
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
class SYSMEL_COMPILER_LIB_EXPORT ScriptModule : public SubtypeOf<ProgramModule, ScriptModule>
{
public:
    static constexpr char const __typeName__[] = "ScriptModule";

    static ScriptModulePtr create();

    virtual bool isScriptModule() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SCRIPT_MODULE_HPP