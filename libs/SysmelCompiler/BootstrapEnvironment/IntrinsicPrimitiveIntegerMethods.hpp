#ifndef INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP
#define INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP

#pragma once

#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFraction.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFloat.hpp"
#include "sysmel/BootstrapEnvironment/PrimitiveIntegerType.hpp"
#include "sysmel/BootstrapEnvironment/PrimitiveCharacterType.hpp"
#include "sysmel/BootstrapEnvironment/PrimitiveFloatType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <string.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
    
template<typename T>
struct IsUnsignedIntegerType
{
    static constexpr bool value = false;
};

template<>
struct IsUnsignedIntegerType<uint8_t>
{
    static constexpr bool value = true;
};

template<>
struct IsUnsignedIntegerType<uint16_t>
{
    static constexpr bool value = true;
};

template<>
struct IsUnsignedIntegerType<uint32_t>
{
    static constexpr bool value = true;
};

template<>
struct IsUnsignedIntegerType<uint64_t>
{
    static constexpr bool value = true;
};

template<typename T>
struct IsCharacterIntegerType
{
    static constexpr bool value = false;
};

template<>
struct IsCharacterIntegerType<char>
{
    static constexpr bool value = true;
};

template<>
struct IsCharacterIntegerType<char16_t>
{
    static constexpr bool value = true;
};

template<>
struct IsCharacterIntegerType<char32_t>
{
    static constexpr bool value = true;
};

template<typename T>
struct IntrinsicPrimitiveIntegerMethods
{
    typedef T PrimitiveInteger;
    typedef ObjectPtr<PrimitiveInteger> PrimitiveIntegerPtr;
    typedef typename PrimitiveInteger::ValueType ValueType;

    static constexpr bool IsUnsigned = IsUnsignedIntegerType<ValueType>::value;
    static constexpr bool IsCharacter = IsCharacterIntegerType<ValueType>::value;
    static constexpr bool IsUnsignedOrCharacter = IsUnsigned || IsCharacter;
    static constexpr bool IsSigned = !IsUnsignedOrCharacter;

    static PrimitiveIntegerPtr makeValue(const ValueType &value)
    {
        auto box = basicMakeObject<PrimitiveInteger> ();
        box->value = value;
        return box;
    }

    static PrimitiveIntegerPtr makeBoolean(const ValueType &value)
    {
        auto box = basicMakeObject<PrimitiveInteger> ();
        box->value = value;
        return box;
    }

    static bool canBeInstantiatedWithLiteralValue(const AnyValuePtr &value)
    {
        if(value->isLiteralInteger())
        {
            auto integerValue = value->unwrapAsLargeInteger();
            return LargeInteger{std::numeric_limits<ValueType>::min()} <= integerValue && integerValue <= LargeInteger{std::numeric_limits<ValueType>::max()};
        }

        return false;
    }

    static PrimitiveIntegerPtr makeWithLargeInteger(const LargeInteger &largeInteger)
    {
        ValueType decoded = 0;
        memcpy(&decoded, largeInteger.words.data(), std::min(sizeof(ValueType), largeInteger.words.size()*4));

        if(largeInteger.isNegative())
            decoded = -decoded;
        return makeValue(decoded);
    }

    static AnyValuePtr instantiateWithLiteralValue(const AnyValuePtr &value)
    {
        if(value->isLiteralInteger())
            return makeWithLargeInteger(value->unwrapAsLargeInteger());

        return nullptr;
    }

    template<typename SourceType>
    static void addConstructorFor(AnyValuePtrList &ctors)
    {
        typedef ObjectPtr<SourceType> SourceTypePtr;
        typedef typename SourceType::ValueType SourceValueType;

        static constexpr bool IsSourceUnsigned = IsUnsignedIntegerType<SourceValueType>::value;
        static constexpr bool IsSourceCharacter = IsCharacterIntegerType<SourceValueType>::value;
        static constexpr bool IsSourceUnsignedOrCharacter = IsSourceUnsigned || IsSourceCharacter;
        static constexpr bool IsSourceSigned = !IsSourceUnsignedOrCharacter;
        static constexpr bool IsSourceSameAsDestination = std::is_same<ValueType, SourceValueType>::value;

        static constexpr auto SourceValueTypeSize = sizeof(SourceValueType);
        static constexpr auto DestinationValueTypeSize = sizeof(ValueType);

        // Ignore the copy constructor.
        if(IsSourceSameAsDestination)
            return;

        std::string intrinsicName;
        bool implicit = false;

        if constexpr(DestinationValueTypeSize < SourceValueTypeSize)
        {
            intrinsicName = "integer.conversion.truncate";
        }
        else if constexpr(DestinationValueTypeSize == SourceValueTypeSize)
        {
            intrinsicName = "integer.conversion.bitcast";
            implicit = IsSourceUnsignedOrCharacter && IsUnsignedOrCharacter;
        }
        else
        {
            static_assert(DestinationValueTypeSize > SourceValueTypeSize, "Increasing size conversion.");
            intrinsicName = IsSourceSigned ? "integer.conversion.sign-extend" : "integer.conversion.zero-extend";
            implicit = !IsSourceSigned || IsSourceSigned == IsSigned;
        }

        ctors.push_back(makeIntrinsicConstructor<PrimitiveIntegerPtr (TypePtr, SourceTypePtr)> (intrinsicName, +[](const TypePtr &, const SourceTypePtr &value){
            return makeValue(ValueType(value->value));
        }, implicit ? MethodFlags::Pure : (MethodFlags::Pure | MethodFlags::Explicit)));
    }

    template<typename SourceType>
    static void addConstructorForFloat(AnyValuePtrList &ctors)
    {
        typedef ObjectPtr<SourceType> SourceTypePtr;

        ctors.push_back(makeIntrinsicConstructor<PrimitiveIntegerPtr (TypePtr, SourceTypePtr)> ("float.conversion.to-integer", +[](const TypePtr &, const SourceTypePtr &value){
            return makeValue(ValueType(value->value));
        }, MethodFlags::Pure | MethodFlags::Explicit));
    }

    static AnyValuePtrList constructors()
    {
        AnyValuePtrList ctors;
        ctors.push_back(makeConstructor<PrimitiveIntegerPtr (TypePtr, LiteralIntegerPtr)> (+[](const TypePtr &, const LiteralIntegerPtr &value){
            return makeWithLargeInteger(value->unwrapAsLargeInteger());
        }, MethodFlags::Explicit | MethodFlags::Pure));
        ctors.push_back(makeConstructor<PrimitiveIntegerPtr (TypePtr, LiteralFractionPtr)> (+[](const TypePtr &, const LiteralFractionPtr &value){
            return makeValue(ValueType(value->unwrapAsFloat64()));
        }, MethodFlags::Explicit | MethodFlags::Pure));
        ctors.push_back(makeConstructor<PrimitiveIntegerPtr (TypePtr, LiteralFloatPtr)> (+[](const TypePtr &, const LiteralFloatPtr &value){
            return makeValue(ValueType(value->unwrapAsFloat64()));
        }, MethodFlags::Explicit | MethodFlags::Pure));

        addConstructorFor<UInt8> (ctors);
        addConstructorFor<UInt16> (ctors);
        addConstructorFor<UInt32> (ctors);
        addConstructorFor<UInt64> (ctors);
        addConstructorFor<Int8> (ctors);
        addConstructorFor<Int16> (ctors);
        addConstructorFor<Int32> (ctors);
        addConstructorFor<Int64> (ctors);
        addConstructorFor<Char8> (ctors);
        addConstructorFor<Char16> (ctors);
        addConstructorFor<Char32> (ctors);
        addConstructorForFloat<Float32> (ctors);
        addConstructorForFloat<Float64> (ctors);
        return ctors;
    }

    static MethodCategories instanceMethods()
    {
        return MethodCategories{
            {"comparisons", {
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.equals", "=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.equals", "==", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.not-equals", "~=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.not-equals", "~~", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.less-than", "<", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value < b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.less-equals", "<=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value <= b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.greater-than", ">", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value > b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.greater-equals", ">=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value >= b->value);
                }, MethodFlags::Pure),
            }},
            {"arithmetic", {
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.neg", "pre--", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(-v->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.neg", "negated", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(-v->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.add", "+", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value + b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.sub", "-", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value - b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.mul", "*", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value * b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.div", "/", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value / b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.mod", "%", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value % b->value);
                }, MethodFlags::Pure),
            }},
            {"bitwise", {
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.bitnot", "pre-~", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(~v->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.bitnot", "bitInvert", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(~v->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitor", "|", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value | b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitor", "bitOr:", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value | b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitand", "&", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value & b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitand", "bitAnd:", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value & b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitxor", "^", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value ^ b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitxor", "bitXor:", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value ^ b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.shift-left", "<<", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value << b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.shift-right", ">>", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value >> b->value);
                }, MethodFlags::Pure),
            }}
        };
    }

    static MethodCategories typeMacroMethods()
    {
        return MethodCategories{
            {"constants", {
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("zero", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(0));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("one", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(1));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("minValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::min()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("negativeInfinityOrMinValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::min()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("maxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::max()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("infinityOrMaxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::min()));
                }, MethodFlags::Macro),
            }}
        };
    }

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP