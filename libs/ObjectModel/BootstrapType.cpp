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
            methodDictionary.insert(std::make_pair(selector, method));
    }
}

TypePtr BootstrapType::getSuperType()
{
    return supertype;
}

AnyValuePtr BootstrapType::lookupSelector(const AnyValuePtr &selector)
{
    auto it = methodDictionary.find(selector);
    if(it != methodDictionary.end())
        return it->second;

    return SuperType::lookupSelector(selector);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius