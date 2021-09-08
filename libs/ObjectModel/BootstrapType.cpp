#include "sysmel/ObjectModel/BootstrapType.hpp"
#include "sysmel/ObjectModel/BootstrapModule.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<BootstrapType> bootstrapTypeTypeRegistration;

void BootstrapType::initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata)
{
    staticMetadata = theStaticMetadata;
    if(staticMetadata->supertype)
        supertype = RuntimeContext::getActive()->getBootstrapModule()->getBootstrapDefinedType(staticMetadata->supertype->bootstrapTypeID);
        
    for(auto &[category, methods] : staticMetadata->instanceMethods())
    {
        for(auto &[selector, method] : methods)
            addMethodWithSelector(selector, method);
    }
}

std::string BootstrapType::printString() const
{
    return staticMetadata->typeName;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius