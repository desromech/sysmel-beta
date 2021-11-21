#include "sysmel/BootstrapEnvironment/FunctionType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FunctionTypeValue> functionalTypeRegistration;

FunctionTypePtr FunctionType::make(const TypePtr &resultType, const TypePtrList &arguments)
{
    auto result = std::make_shared<FunctionType> ();
    result->setSupertypeAndImplicitMetaType(FunctionTypeValue::__staticType__());
    result->arguments = arguments;
    result->result = resultType;
    return result;
}

FunctionTypePtr FunctionType::copyWithResultType(const TypePtr &newResultType)
{
    return make(newResultType, arguments);
}

FunctionTypePtr getOrCreateFunctionType(const TypePtr &resultType, const TypePtrList &arguments)
{
    return FunctionType::make(resultType, arguments);
}

bool FunctionType::isFunctionType() const
{
    return true;
}

FunctionalTypeValuePtr FunctionType::makeValueWithImplementation(const AnyValuePtr &implementation)
{
    auto result = std::make_shared<FunctionTypeValue> ();
    result->type = shared_from_this();
    result->functionalImplementation = implementation;
    return result;
}

bool FunctionTypeValue::isFunctionTypeValue() const
{
    return true;
}

TypePtr FunctionTypeValue::getType() const
{
    return type;
}

AnyValuePtr FunctionTypeValue::applyWithArguments(const std::vector<AnyValuePtr> &arguments)
{
    return functionalImplementation->applyWithArguments(arguments);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius