#ifndef INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP
#define INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP

#pragma once

#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
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

    static AnyValuePtr instantiateWithLiteralValue(const AnyValuePtr &value)
    {
        if(value->isLiteralInteger())
        {
            auto integerValue = value->unwrapAsLargeInteger();
            ValueType decoded = 0;
            memcpy(&decoded, integerValue.words.data(), integerValue.words.size()*4);

            if(integerValue.isNegative())
                decoded = -decoded;
            return makeValue(decoded);
        }

        return nullptr;
    }

    static MethodCategories instanceMethods()
    {
        return MethodCategories{
            {"comparisons", {
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.equals", "=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.equals", "==", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.not-equals", "~=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.not-equals", "~~", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.less-than", "<", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value < b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.less-equals", "<=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value <= b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.greater-than", ">", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value > b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.greater-equals", ">=", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return Boolean8::make(a->value >= b->value);
                }),
            }},
            {"arithmetic", {
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.neg", "pre--", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(-v->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.neg", "negated", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(-v->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.add", "+", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value + b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.sub", "-", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value - b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.mul", "*", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value * b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.div", "/", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value / b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.mod", "%", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value % b->value);
                }),
            }},
            {"bitwise", {
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.bitnot", "pre-~", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(~v->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr)> ("integer.bitnot", "bitInvert", +[](const PrimitiveIntegerPtr &v) {
                    return makeValue(~v->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitor", "|", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value | b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitor", "bitOr:", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value | b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitand", "&", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value & b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitand", "bitAnd:", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value & b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitxor", "^", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value ^ b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.bitxor", "bitXor:", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value ^ b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.shift-left", "<<", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value << b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveIntegerPtr (PrimitiveIntegerPtr, PrimitiveIntegerPtr)> ("integer.shift-right", ">>", +[](const PrimitiveIntegerPtr &a, const PrimitiveIntegerPtr &b) {
                    return makeValue(a->value >> b->value);
                }),
            }}
        };
    }

    static MethodCategories typeMacroMethods()
    {
        return MethodCategories{
            {"constants", {
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("zero", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(0));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("one", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(1));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("minValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::min()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("negativeInfinityOrMinValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::min()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("maxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::max()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("infinityOrMaxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::min()));
                }),
            }}
        };
    }

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP