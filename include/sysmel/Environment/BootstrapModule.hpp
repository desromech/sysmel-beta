#ifndef SYSMEL_ENVIRONMENT_BOOTSTRAP_MODULE_HPP
#define SYSMEL_ENVIRONMENT_BOOTSTRAP_MODULE_HPP
#pragma once

#include "Module.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapModule);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Namespace);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAModule);

/**
 * I am the bootstrap module of the system. I am a
 * special module that holds exclusively objects that are defined by the runtime execution model.
 */
class SYSMEL_COMPILER_LIB_EXPORT BootstrapModule : public SubtypeOf<Module, BootstrapModule>
{
public:
    static constexpr char const __typeName__[] = "BootstrapModule";

    virtual void initialize() override;
    virtual bool isBootstrapModule() const override;
    virtual AnyValuePtr getName() const override;
    
    virtual NamespacePtr getGlobalNamespace() const override;

    TypePtr getBootstrapDefinedTypeNamed(const std::string &typeName);
    TypePtr getBootstrapDefinedType(StaticBootstrapDefinedTypeMetadata *metadata);

    const NamespacePtr &getEnvironmentNamespace() const
    {
        return bootstrapEnvironmentNamespace;
    }

    const NamespacePtr &getEnvironmentSysmelLanguageNamespace() const
    {
        return bootstrapEnvironmentSysmelLanguageNamespace;
    }

    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;

private:
    void initializePrimitiveVectorTypes();
    
    std::vector<TypePtr> bootstrapDefinedTypeTable;
    std::unordered_map<std::string, TypePtr> bootstrapDefinedTypeNameMap;
    std::unordered_map<std::string, size_t> bootstrapDefinedTypeIndexMap;

    AnyValuePtr name;
    NamespacePtr globalNamespace;
    NamespacePtr bootstrapEnvironmentNamespace;
    NamespacePtr bootstrapEnvironmentSysmelLanguageNamespace;
    SSAModulePtr ssaModule;
};



} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_BOOTSTRAP_MODULE_HPP