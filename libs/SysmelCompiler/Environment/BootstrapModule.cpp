#include "Environment/BootstrapModule.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/MetaType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapType.hpp"
#include "Environment/BootstrapExtensionMethods.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/Namespace.hpp"
#include "Environment/SSAModule.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<BootstrapModule> bootstrapModuleTypeRegistration;

void BootstrapModule::initialize()
{
    name = internSymbol("SCDB");
    auto &bootstrapMetadataList = getBootstrapDefinedTypeMetadataList();

    // First pass: create all of the bootstrap type instances.
    bootstrapDefinedTypeTable.reserve(bootstrapDefinedTypeTable.size());
    for(const auto &metadata : bootstrapMetadataList)
    {
        auto type = basicMakeObject<BootstrapType> ();
        auto metaType = basicMakeObject<MetaType> ();
        type->setType(metaType);
        metaType->setThisType(type);

        auto index = bootstrapDefinedTypeTable.size() + 1;
        bootstrapDefinedTypeTable.push_back(type);
        bootstrapDefinedTypeTable.push_back(metaType);
        if(!metadata->typeName.empty())
        {
            bootstrapDefinedTypeNameMap.insert({metadata->typeName, type});
            bootstrapDefinedTypeIndexMap.insert({metadata->typeName, index});
        }
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
        extensionMethods.applyToType(getBootstrapDefinedType(typeMetadata));
    }

    // Create namespaces.
    globalNamespace = Namespace::makeWithName(nullptr);
    globalNamespace->setParentProgramEntity(selfFromThis());

    // Create the bootstrap environemnt namespace.
    bootstrapEnvironmentNamespace = Namespace::makeWithName(internSymbol("__Environment__"));
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

        auto bootstrapType = getBootstrapDefinedType(metadata);
        sysmelAssert(bootstrapType);
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

    // Make the primitive vector types.
    initializePrimitiveVectorTypes();
}

void BootstrapModule::initializePrimitiveVectorTypes()
{
    TypePtr primitiveTypes[] = {
        Boolean8::__staticType__(),

        Int8::__staticType__(),
        Int16::__staticType__(),
        Int32::__staticType__(),
        Int64::__staticType__(),

        UInt8::__staticType__(),
        UInt16::__staticType__(),
        UInt32::__staticType__(),
        UInt64::__staticType__(),

        Char8::__staticType__(),
        Char16::__staticType__(),
        Char32::__staticType__(),

        Float16::__staticType__(),
        Float32::__staticType__(),
        Float64::__staticType__(),
    };
    uint32_t primitiveVectorTypeElements[] = {
        2, 3, 4
    };

    // Synthethize the basic primitive vector types.
    for(const auto &elementType : primitiveTypes)
    {
        for(auto elements : primitiveVectorTypeElements)
        {
            std::ostringstream out;
            out << elementType->getValidNameString() << "x" << elements;
            auto name = out.str();
            auto nameSymbol = internSymbol(name);

            auto vectorType = PrimitiveVectorType::make(elementType, elements);
            vectorType->setName(nameSymbol);

            bootstrapEnvironmentNamespace->recordChildProgramEntityDefinition(vectorType);
            bootstrapEnvironmentNamespace->bindProgramEntityWithVisibility(vectorType, ProgramEntityVisibility::Public);
            bootstrapEnvironmentSysmelLanguageNamespace->bindSymbolWithVisibility(nameSymbol, ProgramEntityVisibility::Public, vectorType);
        }
    }
}

bool BootstrapModule::isBootstrapModule() const
{
    return true;
}

AnyValuePtr BootstrapModule::getName() const
{
    return name;
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

TypePtr BootstrapModule::getBootstrapDefinedType(StaticBootstrapDefinedTypeMetadata *metadata)
{
    if(!metadata)
        return TypePtr();

    if(metadata->bootstrapTypeID == 0 && !metadata->typeName.empty())
    {
        auto it = bootstrapDefinedTypeIndexMap.find(metadata->typeName);
        if(it != bootstrapDefinedTypeIndexMap.end())
            metadata->bootstrapTypeID = it->second;
    }
    
    auto typeID = metadata->bootstrapTypeID;
    if(typeID == 0 || typeID > bootstrapDefinedTypeTable.size())
        return TypePtr();
    return bootstrapDefinedTypeTable[typeID - 1];
}

TypePtr getBootstrapDefinedTypeWithMetadata(StaticBootstrapDefinedTypeMetadata *metadata)
{
    return RuntimeContext::getActive()->getBootstrapModule()->getBootstrapDefinedType(metadata);
}

SSAValuePtr BootstrapModule::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &position)
{
    if(!ssaModule)
    {
        ssaModule = basicMakeObject<SSAModule> ();
        ssaModule->setName(getValidName());
        ssaModule->setGlobalNamespace(globalNamespace->asSSAValueRequiredInPosition(position));
    }
    return ssaModule;
}

} // End of namespace Environment
} // End of namespace Sysmel