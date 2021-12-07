#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/MetaType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapExtensionMethods.hpp"
#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/SSAModule.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<BootstrapModule> bootstrapModuleTypeRegistration;

void BootstrapModule::initialize()
{
    auto &bootstrapMetadataList = getBootstrapDefinedTypeMetadataList();

    // First pass: create all of the bootstrap type instances.
    bootstrapDefinedTypeTable.reserve(bootstrapDefinedTypeTable.size());
    for(const auto &metadata : bootstrapMetadataList)
    {
        auto type = basicMakeObject<BootstrapType> ();
        auto metaType = basicMakeObject<MetaType> ();
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
        auto type = staticObjectCast<BootstrapType> (bootstrapDefinedTypeTable[i*2]);
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

    // Create namespaces.
    globalNamespace = Namespace::makeWithName(nullptr);
    globalNamespace->setParentProgramEntity(selfFromThis());

    // Create the bootstrap environemnt namespace.
    bootstrapEnvironmentNamespace = Namespace::makeWithName(internSymbol("__BootstrapEnvironment__"));
    globalNamespace->recordChildProgramEntityDefinition(bootstrapEnvironmentNamespace);
    globalNamespace->bindProgramEntityWithVisibility(bootstrapEnvironmentNamespace, ProgramEntityVisibility::Public);

    // Create the bootstrap environment sysmel language namespace.
    bootstrapEnvironmentSysmelLanguageNamespace = Namespace::makeWithName(internSymbol("SysmelLanguage"));
    bootstrapEnvironmentNamespace->recordChildProgramEntityDefinition(bootstrapEnvironmentSysmelLanguageNamespace);
    bootstrapEnvironmentNamespace->bindProgramEntityWithVisibility(bootstrapEnvironmentSysmelLanguageNamespace, ProgramEntityVisibility::Public);

    // Register the bootstrap types on the namespaces.
    for(const auto &metadata : bootstrapMetadataList)
    {
        if(metadata->typeName.empty())
            continue;

        auto bootstrapType = getBootstrapDefinedType(metadata->bootstrapTypeID);
        assert(bootstrapType);
        bootstrapEnvironmentNamespace->recordChildProgramEntityDefinition(bootstrapType);
        bootstrapEnvironmentNamespace->bindProgramEntityWithVisibility(bootstrapType, ProgramEntityVisibility::Public);

        if(!metadata->sysmelLanguageTopLevelName.empty())
            bootstrapEnvironmentSysmelLanguageNamespace->bindSymbolWithVisibility(internSymbol(metadata->sysmelLanguageTopLevelName), ProgramEntityVisibility::Public, bootstrapType);
    }

    // IntPointer and UIntPointer
    bootstrapEnvironmentNamespace->bindSymbolWithVisibility(internSymbol("IntPointer"), ProgramEntityVisibility::Public, Type::getIntPointerType());
    bootstrapEnvironmentSysmelLanguageNamespace->bindSymbolWithVisibility(internSymbol("IntPointer"), ProgramEntityVisibility::Public, Type::getIntPointerType());

    bootstrapEnvironmentNamespace->bindSymbolWithVisibility(internSymbol("UIntPointer"), ProgramEntityVisibility::Public, Type::getUIntPointerType());
    bootstrapEnvironmentSysmelLanguageNamespace->bindSymbolWithVisibility(internSymbol("UIntPointer"), ProgramEntityVisibility::Public, Type::getUIntPointerType());
}

bool BootstrapModule::isBootstrapModule() const
{
    return true;
}

NamespacePtr BootstrapModule::getGlobalNamespace() const
{
    return globalNamespace;
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

SSAValuePtr BootstrapModule::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &position)
{
    if(!ssaModule)
    {
        ssaModule = basicMakeObject<SSAModule> ();
        ssaModule->setGlobalNamespace(globalNamespace->asSSAValueRequiredInPosition(position));
    }
    return ssaModule;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius