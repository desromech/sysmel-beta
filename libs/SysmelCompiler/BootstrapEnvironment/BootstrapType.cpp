#include "sysmel/BootstrapEnvironment/BootstrapType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

bool BootstrapType::isBootstrapType() const
{
    return true;
}

bool BootstrapType::supportsDynamicCompileTimeMessageSend() const
{
    return staticMetadata->isDynamicCompileTimeType;
}

bool BootstrapType::supportsMessageAnalysisByLiteralValueReceivers() const
{
    return staticMetadata->isLiteralValueMessageAnalyzer;
}

bool BootstrapType::isEphemeralCompileTimeObject() const
{
    return staticMetadata->isEphemeralCompileTimeObject;
}

void BootstrapType::initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata)
{
    staticMetadata = theStaticMetadata;
    if(!staticMetadata->typeName.empty())
        name = internSymbol(staticMetadata->typeName);

    if(staticMetadata->supertype)
    {
        supertype = RuntimeContext::getActive()->getBootstrapModule()->getBootstrapDefinedType(staticMetadata->supertype->bootstrapTypeID);
        metaType->setSupertype(supertype->getType());
    }
        
    addMacroMethodCategories(staticMetadata->instanceMacroMethods());
    addMethodCategories(staticMetadata->instanceMethods());
    addDefaultTypeConversionRules();

    metaType->addMacroMethodCategories(staticMetadata->typeMacroMethods());
    metaType->addMethodCategories(staticMetadata->typeMethods());
    metaType->addDefaultTypeConversionRules();
}

std::string BootstrapType::printString() const
{
    return staticMetadata->typeName;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius