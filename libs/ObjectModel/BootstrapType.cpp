#include "sysmel/ObjectModel/BootstrapType.hpp"
#include "sysmel/ObjectModel/BootstrapModule.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<BootstrapType> bootstrapTypeTypeRegistration;

bool BootstrapType::isBootstrapType() const
{
    return true;
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius