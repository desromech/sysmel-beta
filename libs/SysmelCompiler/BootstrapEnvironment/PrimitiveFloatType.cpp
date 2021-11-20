#include "sysmel/BootstrapEnvironment/PrimitiveFloatType.hpp"
#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveFloatType> PrimitiveFloatTypeTypeRegistration;
static BootstrapTypeRegistration<Float16> Float16TypeRegistration;
static BootstrapTypeRegistration<Float32> Float32TypeRegistration;
static BootstrapTypeRegistration<Float64> Float64TypeRegistration;

template<typename T>
struct IntrinsicPrimitiveFloatMethods
{
    typedef T PrimitiveFloat;
    typedef std::shared_ptr<PrimitiveFloat> PrimitiveFloatPtr;
    typedef typename PrimitiveFloat::ValueType ValueType;

    static PrimitiveFloatPtr makeValue(const ValueType &value)
    {
        auto box = std::make_shared<PrimitiveFloat> ();
        box->value = value;
        return box;
    }

    static MethodCategories instanceMethods()
    {
        return MethodCategories{
            {"comparisons", {
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.equals", "=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.equals", "==", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.not-equals", "~=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.not-equals", "~~", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.less-than", "<", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value < b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.less-equals", "<=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value <= b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.greater-than", ">", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value > b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("integer.greater-equals", ">=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value >= b->value);
                }),
            }},
            {"arithmetic", {
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.neg", "negated", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(-v->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.add", "+", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value + b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.sub", "-", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value - b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.mul", "*", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value * b->value);
                }),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.div", "/", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value / b->value);
                }),
            }}
        };
    }
};

bool PrimitiveFloatType::isPrimitiveFloatTypeValue() const
{
    return true;
}

MethodCategories Float32::__instanceMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float32>::instanceMethods();
}

MethodCategories Float64::__instanceMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float64>::instanceMethods();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius