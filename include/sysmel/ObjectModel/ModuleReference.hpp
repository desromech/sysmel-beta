#ifndef SYSMEL_COMPILER_OBJECT_MODEL_MODULE_REFERENCE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_MODULE_REFERENCE_HPP
#pragma once

#include "AnyValue.hpp"
#include "LiteralSymbol.hpp"
#include "Module.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a reference towards a particular module in the system
 */
class ModuleReference : public SubtypeOf<AnyValue, ModuleReference>
{
public:
    static constexpr char const __typeName__[] = "ModuleReference";

protected:
    LiteralSymbolPtr name; 
    ModulePtr solvedModule;
};

typedef std::shared_ptr<ModuleReference> ModuleReferencePtr;

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_MODULE_REFERENCE_HPP