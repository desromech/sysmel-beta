#include "Environment/BootstrapType.hpp"
#include "Environment/BootstrapModule.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

bool BootstrapType::isBootstrapType() const
{
    return true;
}

AnyValuePtr BootstrapType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitBootstrapType(selfFromThis());
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

bool BootstrapType::isNullableType() const
{
    return staticMetadata->isNullableType;
}

bool BootstrapType::isImmutableType()
{
    return staticMetadata && staticMetadata->isImmutableType;
}

bool BootstrapType::hasTrivialInitialization()
{
    return staticMetadata->hasTrivialInitialization;
}

bool BootstrapType::hasTrivialInitializationCopyingFrom()
{
    return staticMetadata->hasTrivialInitializationCopyingFrom;
}

bool BootstrapType::hasTrivialInitializationMovingFrom()
{
    return staticMetadata->hasTrivialInitializationMovingFrom;
}

bool BootstrapType::hasTrivialFinalization()
{
    return staticMetadata->hasTrivialFinalization;
}

bool BootstrapType::hasTrivialCopyingFrom()
{
    return staticMetadata->hasTrivialCopyingFrom;
}

bool BootstrapType::hasTrivialMovingFrom()
{
    return staticMetadata->hasTrivialMovingFrom;
}

uint64_t BootstrapType::getMemorySize()
{
    return staticMetadata->memorySize;
}

uint64_t BootstrapType::getMemoryAlignment()
{
    return staticMetadata->memoryAlignment();
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
        
    addFieldVariables(staticMetadata->fieldVariables());
    addMacroMethodCategories(staticMetadata->instanceMacroMethods());
    addMethodCategories(staticMetadata->instanceMethods());
    addConstructors(staticMetadata->constructors());
    addConversions(staticMetadata->conversions());
    addDefaultTypeConversionRules();

    metaType->addMacroMethodCategories(staticMetadata->typeMacroMethods());
    metaType->addMethodCategories(staticMetadata->typeMethods());
    metaType->addDefaultTypeConversionRules();
}

void BootstrapType::addDefaultTypeConversionRules()
{
    SuperType::addDefaultTypeConversionRules();
    staticMetadata->addTypeConversionRules(selfFromThis());
}

std::string BootstrapType::printString() const
{
    return staticMetadata->typeName;
}

AnyValuePtr BootstrapType::basicNewValue()
{
    return staticMetadata->basicNewValue();
}

AnyValuePtr BootstrapType::shallowCloneInstance(const AnyValuePtr &cloned)
{
    return staticMetadata->shallowClone(cloned);
}

bool BootstrapType::canBeInstantiatedWithLiteralValue(const AnyValuePtr &value)
{
    return staticMetadata->canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr BootstrapType::instantiatedWithLiteralValue(const AnyValuePtr &value)
{
    return staticMetadata->instantiateWithLiteralValue(value);
}

TypePtr BootstrapType::asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    return staticMetadata->asInferredTypeForWithModeInEnvironment(selfFromThis(), node, mode, isMutable, concreteLiterals, environment);
}

} // End of namespace Environment
} // End of namespace Sysmel