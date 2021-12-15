#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

namespace Sysmel
{
namespace Environment
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

    auto result = basicMakeObject<PointerType> ();
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
    auto pointer = basicMakeObject<PointerTypeValue> ();
    pointer->type = selfFromThis();
    pointer->baseValue = value;
    return pointer;
}

void PointerType::addSpecializedInstanceMethods()
{
    auto referenceType = baseType->refFor(addressSpace);

    addMethodCategories(MethodCategories{
            {"accessing", {
                // value/_
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr)> ("pointer.to.reference", "value", selfFromThis(), referenceType, {}, [=](const PointerTypeValuePtr &value) {
                    return referenceType->makeWithValue(value->baseValue);
                }, MethodFlags::Pure),

                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr)> ("pointer.to.reference", "_", selfFromThis(), referenceType, {}, [=](const PointerTypeValuePtr &value) {
                    return referenceType->makeWithValue(value->baseValue);
                }, MethodFlags::Pure),

                // []
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr, AnyValuePtr)> ("pointer.element", "[]", selfFromThis(), referenceType, {Type::getIntPointerType()}, [=](const PointerTypeValuePtr &value, const AnyValuePtr &index) {
                    return value->baseValue->getReferenceToSlotWithType(index->unwrapAsInt64(), referenceType);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr, AnyValuePtr)> ("pointer.element", "[]", selfFromThis(), referenceType, {Type::getUIntPointerType()}, [=](const PointerTypeValuePtr &value, const AnyValuePtr &index) {
                    return value->baseValue->getReferenceToSlotWithType(index->unwrapAsUInt64(), referenceType);
                }, MethodFlags::Pure),
            }
        }
    });
}

AnyValuePtr PointerType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitPointerType(selfFromThis());
}

bool PointerTypeValue::isPointerTypeValue() const
{
    return true;
}

TypePtr PointerTypeValue::getType() const
{
    return type;
}

} // End of namespace Environment
} // End of namespace Sysmel