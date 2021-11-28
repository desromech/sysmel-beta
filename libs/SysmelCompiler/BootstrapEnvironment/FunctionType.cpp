#include "sysmel/BootstrapEnvironment/FunctionType.hpp"
#include "sysmel/BootstrapEnvironment/ClosureType.hpp"
#include "sysmel/BootstrapEnvironment/MethodType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FunctionTypeValue> functionalTypeRegistration;

FunctionTypePtr FunctionType::make(const TypePtr &resultType, const TypePtrList &arguments)
{
    auto canonicalResultType = resultType->asUndecoratedType();
    auto canonicalArgumentTypes = arguments;
    for(auto &el : canonicalArgumentTypes)
        el = el->asUndecoratedType();

    auto &cache = RuntimeContext::getActive()->functionTypeCache;
    auto it = cache.find({canonicalResultType, canonicalArgumentTypes});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<FunctionType> ();
    result->setSupertypeAndImplicitMetaType(FunctionTypeValue::__staticType__());
    result->arguments = canonicalArgumentTypes;
    result->result = canonicalResultType;
    cache.insert({{canonicalResultType, canonicalArgumentTypes}, result});
    result->addSpecializedMethods();
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
    auto result = basicMakeObject<FunctionTypeValue> ();
    result->type = selfFromThis();
    result->functionalImplementation = implementation;
    return result;
}

void FunctionType::addSpecializedMethods()
{
    getType()->addMethodCategories(MethodCategories{
        {"type composition", {
            makeMethodBinding<TypePtr (TypePtr)> ("closure", +[](const TypePtr &self) {
                auto selfFunctionType = staticObjectCast<FunctionType> (self);
                return ClosureType::make(selfFunctionType->result, selfFunctionType->arguments);
            }, MethodFlags::Pure),
            makeMethodBinding<TypePtr (TypePtr, TypePtr)> ("methodWithReceiver:", +[](const TypePtr &self, const TypePtr &receiverType) {
                auto selfFunctionType = staticObjectCast<FunctionType> (self);
                return MethodType::make(receiverType, selfFunctionType->result, selfFunctionType->arguments);
            }, MethodFlags::Pure),
        }}
    });
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