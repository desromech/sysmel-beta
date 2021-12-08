#ifndef SYSMEL_ENVIRONMENT_MODULE_REFERENCE_HPP
#define SYSMEL_ENVIRONMENT_MODULE_REFERENCE_HPP
#pragma once

#include "CompilerObject.hpp"
#include "LiteralSymbol.hpp"
#include "Module.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_MODULE_REFERENCE_HPP