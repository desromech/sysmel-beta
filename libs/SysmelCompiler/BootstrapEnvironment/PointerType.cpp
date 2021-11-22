#include "sysmel/BootstrapEnvironment/PointerType.hpp"
#include "sysmel/BootstrapEnvironment/ReferenceType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PointerTypeValue> PointerTypeValueTypeRegistration;

PointerTypePtr PointerType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, internSymbol("generic"));
}

PointerTypePtr PointerType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto &cache = RuntimeContext::getActive()->pointerTypeCache;
    auto it = cache.find({baseType, addressSpace});
    if(it != cache.end())
        return it->second;

    auto result = std::make_shared<PointerType> ();
    result->setSupertypeAndImplicitMetaType(PointerTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = addressSpace;
    cache.insert({{baseType, addressSpace}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool PointerType::isPointerType() const
{
    return true;
}

std::string PointerType::printString() const
{
    if(hasGenericAddressSpace())
        return baseType->printString() + " pointer";
    return "(" + baseType->printString() + " pointerFor: " + addressSpace->printString();
}

SExpression PointerType::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"pointerType"}},
        baseType->asSExpression(),
        addressSpace->asSExpression()
    }};
}

PointerLikeTypeValuePtr PointerType::makeWithValue(const AnyValuePtr &value)
{
    auto pointer = std::make_shared<PointerTypeValue> ();
    pointer->type = shared_from_this();
    pointer->baseValue = value;
    return pointer;
}

void PointerType::addSpecializedInstanceMethods()
{
    auto referenceType = baseType->refFor(addressSpace);

    addMethodCategories(MethodCategories{
            {"accessing", {
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr)> ("pointer.to.reference", "value", shared_from_this(), referenceType, {}, [=](const PointerTypeValuePtr &value) {
                    return referenceType->makeWithValue(value->baseValue);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr)> ("pointer.to.reference", "_", shared_from_this(), referenceType, {}, [=](const PointerTypeValuePtr &value) {
                    return referenceType->makeWithValue(value->baseValue);
                }, MethodFlags::Pure),
            }
        }
    });
}

bool PointerTypeValue::isPointerTypeValue() const
{
    return true;
}

TypePtr PointerTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius