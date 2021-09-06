#include "sysmel/ObjectModel/BootstrapModule.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapType.hpp"
#include <iostream>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<BootstrapModule> bootstrapModuleTypeRegistration;

void BootstrapModule::initialize()
{
    auto &bootstrapMetadataList = getBootstrapDefinedTypeMetadataList();

    // First pass: create all of the bootstrap type instances.
    for(auto metadata : bootstrapMetadataList)
    {
        auto type = std::make_shared<BootstrapType> ();
        bootstrapDefinedTypeMetadataMap.insert(std::make_pair(metadata, type));
        if(!metadata->typeName.empty())
            bootstrapDefinedTypeNameMap.insert(std::make_pair(metadata->typeName, type));
    }

    // Second pass: initialize the bootstrap types.
    for(auto metadata : bootstrapMetadataList)
    {
        auto type = std::static_pointer_cast<BootstrapType> (getBootstrapDefinedType(metadata));
        type->initializeWithMetadata(metadata);
    }
}

bool BootstrapModule::isBootstrapModule() const
{
    return true;
}

TypePtr BootstrapModule::getBootstrapDefinedTypeNamed(const std::string &typeName)
{
    auto it = bootstrapDefinedTypeNameMap.find(typeName);
    return it != bootstrapDefinedTypeNameMap.end() ? it->second : TypePtr();    
}

TypePtr BootstrapModule::getBootstrapDefinedType(const StaticBootstrapDefinedTypeMetadata *metadata)
{
    auto it = bootstrapDefinedTypeMetadataMap.find(metadata);
    return it != bootstrapDefinedTypeMetadataMap.end() ? it->second : TypePtr();
}

TypePtr getBootstrapDefinedTypeWithMetadata(const StaticBootstrapDefinedTypeMetadata *metadata)
{
    return RuntimeContext::getActive()->getBootstrapModule()->getBootstrapDefinedType(metadata);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius