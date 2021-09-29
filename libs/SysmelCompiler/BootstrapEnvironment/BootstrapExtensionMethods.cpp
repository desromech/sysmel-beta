#include "sysmel/BootstrapEnvironment/BootstrapExtensionMethods.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include <assert.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

typedef std::shared_ptr<BootstrapExtensionMethodsList> BootstrapExtensionMethodsListPtr;

static BootstrapExtensionMethodsListPtr getOrCreateExtensionMethodList()
{
    static BootstrapExtensionMethodsListPtr singleton = std::make_shared<BootstrapExtensionMethodsList> ();
    return singleton;
}

const BootstrapExtensionMethodsList &getRegisteredBootstrapExtensionMethods()
{
    return *getOrCreateExtensionMethodList();
}

void registerBootstrapExtensionMethods(StaticBootstrapDefinedTypeMetadata *typeMetadata, const BootstrapExtensionMethods &extensionMethods)
{
    getOrCreateExtensionMethodList()->push_back(std::make_pair(typeMetadata, extensionMethods));
}

void BootstrapExtensionMethods::applyToType(const TypePtr &type) const
{
    if(instanceMacroMethodsDescriptionFunction)
        type->addMacroMethodCategories(instanceMacroMethodsDescriptionFunction());
    if(instanceMethodsDescriptionFunction)
        type->addMethodCategories(instanceMethodsDescriptionFunction());

    auto metaType = type->getType();
    if(metaType)
    {
        if(typeMacroMethodsDescriptionFunction)
            type->addMacroMethodCategories(typeMacroMethodsDescriptionFunction());
        if(typeMethodsDescriptionFunction)
            type->addMethodCategories(typeMethodsDescriptionFunction());
    }
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius