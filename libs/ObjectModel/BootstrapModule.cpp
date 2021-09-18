#include "sysmel/ObjectModel/BootstrapModule.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/MetaType.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapType.hpp"
#include "sysmel/ObjectModel/BootstrapExtensionMethods.hpp"
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
    bootstrapDefinedTypeTable.reserve(bootstrapDefinedTypeTable.size());
    for(auto metadata : bootstrapMetadataList)
    {
        auto type = std::make_shared<BootstrapType> ();
        auto metaType = std::make_shared<MetaType> ();
        type->setType(metaType);
        metaType->setThisType(type);

        bootstrapDefinedTypeTable.push_back(type);
        bootstrapDefinedTypeTable.push_back(metaType);
        if(!metadata->typeName.empty())
            bootstrapDefinedTypeNameMap.insert(std::make_pair(metadata->typeName, type));
    }

    // Second pass: initialize the bootstrap types.
    for(size_t i = 0; i < bootstrapMetadataList.size(); ++i)
    {
        auto type = std::static_pointer_cast<BootstrapType> (bootstrapDefinedTypeTable[i*2]);
        type->initializeWithMetadata(bootstrapMetadataList[i]);
    }

    // Special case: short-circuit any value type with the type.
    // We are skipping over BootstrapType because not everying is going to be one of them.
    AnyValue::__staticType__()->getType()->setSupertype(Type::__staticType__());

    // Register each type with its supertype.
    for(const auto &type : bootstrapDefinedTypeTable)
    {
        auto supertype = type->getSupertype();
        if(supertype)
            supertype->registerSubtype(type);
    }

    // Third pass: apply the extension methods.
    for(const auto &[typeMetadata, extensionMethods] : getRegisteredBootstrapExtensionMethods())
    {
        extensionMethods.applyToType(getBootstrapDefinedType(typeMetadata->bootstrapTypeID));
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

TypePtr BootstrapModule::getBootstrapDefinedType(size_t bootstrapTypeID)
{
    if(bootstrapTypeID == 0 || bootstrapTypeID > bootstrapDefinedTypeTable.size())
        return TypePtr();
    return bootstrapDefinedTypeTable[bootstrapTypeID - 1];
}

TypePtr getBootstrapDefinedTypeWithID(size_t bootstrapTypeID)
{
    return RuntimeContext::getActive()->getBootstrapModule()->getBootstrapDefinedType(bootstrapTypeID);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius