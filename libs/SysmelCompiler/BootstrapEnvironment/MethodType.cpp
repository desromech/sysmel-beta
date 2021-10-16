#include "sysmel/BootstrapEnvironment/MethodType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<MethodTypeValue> functionalTypeRegistration;

MethodTypePtr MethodType::make(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments)
{
    auto result = std::make_shared<MethodType> ();
    result->receiverType = receiverType;
    result->arguments = arguments;
    result->result = resultType;
    result->setSupertypeAndImplicitMetaType(MethodTypeValue::__staticType__());
    return result;
}

TypePtr MethodType::getReceiverType() const
{
    return receiverType;
}


FunctionalTypeValuePtr MethodType::makeValueWithImplementation(const AnyValuePtr &implementation)
{
    auto result = std::make_shared<MethodTypeValue> ();
    result->type = shared_from_this();
    result->functionalImplementation = implementation;
    return result;
}

MethodTypePtr getOrCreateMethodType(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments)
{
    return MethodType::make(receiverType, resultType, arguments);
}

bool MethodType::isMethodType() const
{
    return true;
}

bool MethodTypeValue::isMethodTypeValue() const
{
    return true;
}

TypePtr MethodTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius