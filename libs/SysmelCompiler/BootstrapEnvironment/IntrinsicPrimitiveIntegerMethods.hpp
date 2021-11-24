#ifndef INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP
#define INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP

#pragma once

#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <string.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
    
template<typename T>
struct IntrinsicPrimitiveIntegerMethods
{
    typedef T PrimitiveInteger;
    typedef std::shared_ptr<PrimitiveInteger> PrimitiveIntegerPtr;
    typedef typename PrimitiveInteger::ValueType ValueType;

    static PrimitiveIntegerPtr makeValue(const ValueType &value)
    {
        auto box = std::make_shared<PrimitiveInteger> ();
        box->value = value;
        return box;
    }

    static PrimitiveIntegerPtr makeBoolean(const ValueType &value)
    {
        auto box = std::make_shared<PrimitiveInteger> ();
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

    static AnyValuePtrList constructors()
    {
        return AnyValuePtrList{
            makeConstructor<PrimitiveIntegerPtr (TypePtr, LiteralIntegerPtr)> (+[](const TypePtr &, const LiteralIntegerPtr &value){
                return makeWithLargeInteger(value->unwrapAsLargeInteger());
            }, MethodFlags::Explicit | MethodFlags::Pure),
        };
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