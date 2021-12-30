#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/LiteralString.hpp"
#include "Environment/IdentityTypeConversionRule.hpp"
#include "Environment/ValueAsVoidTypeConversionRule.hpp"
#include "Environment/NotInCompileTimeError.hpp"
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

    // LiteralString -> CString
    if(baseType->isConstDecoratedType())
    {
        auto nonConstBaseType = baseType->asUndecoratedType();
        if(nonConstBaseType == Char8::__staticType__() || nonConstBaseType == Char16::__staticType__() || nonConstBaseType == Char32::__staticType__())
        {
            addConstructor(makeIntrinsicConstructorWithSignature<PointerTypeValuePtr (PointerTypePtr, LiteralStringPtr)> ("pointer.for.cstring", getType(), selfFromThis(), {LiteralString::__staticType__()}, [=](const PointerTypePtr &self, const LiteralStringPtr &literalString) {
                return self->makeWithValue(literalString);
            }, MethodFlags::Pure));
        }
    }

    auto booleanType = Boolean8::__staticType__();
    addMethodCategories(MethodCategories{
            {"comparisons", {
                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.equals", "=", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),
                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.equals", "==", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),

                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.not-equals", "~=", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),
                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.not-equals", "~~", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),

                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.less-than", "<", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),
                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.less-equals", "<=", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),
                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.greater-than", ">", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),
                makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (PointerTypeValuePtr, PointerTypeValuePtr)> ("pointer.greater-equals", ">=", selfFromThis(), booleanType, {selfFromThis()}, [=](const PointerTypeValuePtr &, const PointerTypeValuePtr &) -> Boolean8Ptr {
                    SysmelSelfNotInCompileTimeError();
                }, MethodFlags::NotInCompileTime),

            }},

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
                    auto indexValue = index->unwrapAsInt64();
                    auto baseType = value->getType().staticAs<PointerLikeType> ()->getBaseType();
                    auto offset = indexValue * baseType->getAlignedMemorySize();
                    return value->baseValue->getReferenceToSlotWithType(indexValue, offset, referenceType);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (PointerTypeValuePtr, AnyValuePtr)> ("pointer.element", "[]", selfFromThis(), referenceType, {Type::getUIntPointerType()}, [=](const PointerTypeValuePtr &value, const AnyValuePtr &index) {
                    auto indexValue = index->unwrapAsUInt64();
                    auto baseType = value->getType().staticAs<PointerLikeType> ()->getBaseType();
                    auto offset = indexValue * baseType->getAlignedMemorySize();
                    return value->baseValue->getReferenceToSlotWithType(indexValue, offset, referenceType);
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

SExpression PointerTypeValue::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"pointerValue"}},
        type->asSExpression(),
        baseValue ? baseValue->asSExpression() : nullptr
    }};
}

AnyValuePtr PointerTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitPointerTypeValue(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel