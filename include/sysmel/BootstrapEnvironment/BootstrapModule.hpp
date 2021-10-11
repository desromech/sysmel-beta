#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_MODULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_MODULE_HPP
#pragma once

#include "Module.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapModule);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Namespace);

/**
 * I am the bootstrap module of the system. I am a
 * special module that holds exclusively objects that are defined by the runtime execution model.
 */
class BootstrapModule : public SubtypeOf<Module, BootstrapModule>
{
public:
    static constexpr char const __typeName__[] = "BootstrapModule";

    virtual void initialize() override;
    virtual bool isBootstrapModule() const override;
    
    virtual NamespacePtr getGlobalNamespace() const override;

    TypePtr getBootstrapDefinedTypeNamed(const std::string &typeName);
    TypePtr getBootstrapDefinedType(size_t id);

    const NamespacePtr &getBootstrapEnvironmentNamespace() const
    {
        return bootstrapEnvironmentNamespace;
    }

    const NamespacePtr &getBootstrapEnvironmentSysmelLanguageNamespace() const
    {
        return bootstrapEnvironmentSysmelLanguageNamespace;
    }

private:
    std::vector<TypePtr> bootstrapDefinedTypeTable;
    std::unordered_map<std::string, TypePtr> bootstrapDefinedTypeNameMap;

    NamespacePtr globalNamespace;
    NamespacePtr bootstrapEnvironmentNamespace;
    NamespacePtr bootstrapEnvironmentSysmelLanguageNamespace;
};



} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_MODULE_HPP