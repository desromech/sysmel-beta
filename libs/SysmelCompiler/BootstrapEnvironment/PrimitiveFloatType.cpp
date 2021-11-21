#include "sysmel/BootstrapEnvironment/PrimitiveFloatType.hpp"
#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
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
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.neg", "pre--", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(-v->value);
                }),
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
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("maxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::max()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("infinity", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::infinity()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("infinityOrMaxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::infinity()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("negativeInfinity", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(-std::numeric_limits<ValueType>::infinity()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("negativeInfinityOrMinValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::infinity()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("nan", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::quiet_NaN()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("qNaN", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::quiet_NaN()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("sNaN", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::signaling_NaN()));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("e", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(2.718281828459045)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("piReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.3183098861837907)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("pi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(3.141592653589793)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("twoPi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(6.283185307179586)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("twoPiReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.15915494309189535)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("threePi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(9.42477796076938)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("threePiReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.1061032953945969)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("halfPi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(1.5707963267948966)));
                }),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("halfPiReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.6366197723675814)));
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

MethodCategories Float32::__typeMacroMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float32>::typeMacroMethods();
}

float Float32::unwrapAsFloat32() const
{
    return value;
}

std::string Float32::asString() const
{
    return castToString(value);
}

std::string Float32::printString() const
{
    return "Float32(" + castToString(value) + ")";
}

SExpression Float32::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"float32"}},
        value
    }};
}

MethodCategories Float64::__instanceMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float64>::instanceMethods();
}

MethodCategories Float64::__typeMacroMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float64>::typeMacroMethods();
}

double Float64::unwrapAsFloat64() const
{
    return value;
}

std::string Float64::asString() const
{
    return castToString(value);
}

std::string Float64::printString() const
{
    return "Float64(" + castToString(value) + ")";
}

SExpression Float64::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"float64"}},
        value
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius