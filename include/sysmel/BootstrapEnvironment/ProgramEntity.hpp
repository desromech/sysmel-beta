#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
class ProgramEntity : public SubtypeOf<CompilerObject, ProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "ProgramEntity";

    virtual bool isProgramEntity() const override;

    virtual ModulePtr getDefinitionModule() const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP