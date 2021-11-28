#include "sysmel/BootstrapEnvironment/MethodType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<MethodTypeValue> functionalTypeRegistration;

MethodTypePtr MethodType::make(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments)
{
    auto canonicalReceiverType = receiverType->asUndecoratedType();
    auto canonicalResultType = resultType->asUndecoratedType();
    auto canonicalArgumentTypes = arguments;
    for(auto &el : canonicalArgumentTypes)
        el = el->asUndecoratedType();

    auto &cache = RuntimeContext::getActive()->methodTypeCache;
    auto it = cache.find({canonicalReceiverType, canonicalResultType, canonicalArgumentTypes});
    if(it != cache.end())
        return it->second;

    auto result = std::make_shared<MethodType> ();
    result->receiverType = canonicalReceiverType;
    result->arguments = canonicalArgumentTypes;
    result->result = canonicalResultType;
    result->setSupertypeAndImplicitMetaType(MethodTypeValue::__staticType__());
    cache.insert({{canonicalReceiverType, canonicalResultType, canonicalArgumentTypes}, result});
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

std::string MethodType::printString() const
{
    return "((" + SuperType::printString() + ") methodWithReceiver: " + receiverType->printString() + ")";
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