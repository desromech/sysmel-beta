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

FunctionTypePtr getOrCreateFunctionType(const TypePtr &resultType, const TypePtrList &arguments)
{
    return FunctionType::make(resultType, arguments);
}

bool FunctionType::isFunctionType() const
{
    return true;
}

TypePtr FunctionType::getType() const
{
    return metaType;
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius