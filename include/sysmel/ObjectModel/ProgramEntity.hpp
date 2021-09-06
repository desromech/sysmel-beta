#ifndef SYSMEL_COMPILER_OBJECT_MODEL_PROGRAM_ENTITY_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_PROGRAM_ENTITY_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class Module;
typedef std::shared_ptr<Module> ModulePtr;

class BootstrapModule;
typedef std::shared_ptr<BootstrapModule> BootstrapModulePtr;

class RuntimeContext;
typedef std::shared_ptr<RuntimeContext> RuntimeContextPtr;

/**
 * I am the base interface for any program metamodel entity
 */
class ProgramEntity : public SubtypeOf<AnyValue, ProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "ProgramEntity";

    virtual ModulePtr getDefinitionModule() const;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_PROGRAM_ENTITY_HPP