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

bool BootstrapType::isNullableType() const
{
    return staticMetadata->isNullableType;
}

bool BootstrapType::isImmutableType()
{
    return staticMetadata->isImmutableType;
}

bool BootstrapType::hasTrivialInitialization()
{
    return staticMetadata->hasTrivialInitialization;
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
    staticMetadata->addTypeConversionRules(shared_from_this());
}

std::string BootstrapType::printString() const
{
    return staticMetadata->typeName;
}

AnyValuePtr BootstrapType::basicNewValue()
{
    return staticMetadata->basicNewValue();
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
    return staticMetadata->asInferredTypeForWithModeInEnvironment(shared_from_this(), node, mode, isMutable, concreteLiterals, environment);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius