#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_REFERENCE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_REFERENCE_HPP
#pragma once

#include "CompilerObject.hpp"
#include "LiteralSymbol.hpp"
#include "Module.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ModuleReference);

/**
 * I am a reference towards a particular module in the system
 */
class ModuleReference : public SubtypeOf<CompilerObject, ModuleReference>
{
public:
    static constexpr char const __typeName__[] = "ModuleReference";

protected:
    LiteralSymbolPtr name; 
    ModulePtr solvedModule;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_REFERENCE_HPP