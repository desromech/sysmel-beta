#include "sysmel/BootstrapEnvironment/BootstrapType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<BootstrapType> bootstrapTypeTypeRegistration;

bool BootstrapType::isBootstrapType() const
{
    return true;
}

bool BootstrapType::supportsDynamicCompileTimeMessageSend() const
{
    return staticMetadata->isDynamicCompileTimeType;
}

void BootstrapType::initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata)
{
    staticMetadata = theStaticMetadata;
    if(staticMetadata->supertype)
    {
        supertype = RuntimeContext::getActive()->getBootstrapModule()->getBootstrapDefinedType(staticMetadata->supertype->bootstrapTypeID);
        metaType->setSupertype(supertype->getType());
    }
        
    addMacroMethodCategories(staticMetadata->instanceMacroMethods());
    addMethodCategories(staticMetadata->instanceMethods());

    metaType->addMacroMethodCategories(staticMetadata->typeMacroMethods());
    metaType->addMethodCategories(staticMetadata->typeMethods());
}

TypePtr BootstrapType::getType() const
{
    return metaType;
}

void BootstrapType::setType(const TypePtr &theMetaType)
{
    metaType = theMetaType;
}

std::string BootstrapType::printString() const
{
    return staticMetadata->typeName;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius