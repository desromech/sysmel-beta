#ifndef INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP
#define INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP

#pragma once

#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"

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
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //INTRINSIC_PRIMITIVE_INTEGER_METHODS_HPP