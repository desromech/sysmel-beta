#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/LiteralInteger.hpp"
#include "Environment/LiteralFloat.hpp"
#include "Environment/LiteralFraction.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/RuntimeContext.hpp"
#include <string.h>
#include <math.h>
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveFloatType> PrimitiveFloatTypeTypeRegistration;
static BootstrapTypeRegistration<Float16> Float16TypeRegistration;
static BootstrapTypeRegistration<Float32> Float32TypeRegistration;
static BootstrapTypeRegistration<Float64> Float64TypeRegistration;

template<typename T>
struct IntrinsicPrimitiveFloatMethods
{
    typedef T PrimitiveFloat;
    typedef ObjectPtr<PrimitiveFloat> PrimitiveFloatPtr;
    typedef typename PrimitiveFloat::ValueType ValueType;

    static PrimitiveFloatPtr makeValue(const ValueType &value)
    {
        auto box = basicMakeObject<PrimitiveFloat> ();
        box->value = value;
        return box;
    }

    static bool canBeInstantiatedWithLiteralValue(const AnyValuePtr &value)
    {
        if(value->isLiteralInteger() || value->isLiteralFraction() || value->isLiteralFloat())
        {
            double floatValue = value->unwrapAsFloat64();
            double truncatedValue = double(ValueType(floatValue));
            return memcmp(&floatValue, &truncatedValue, 8) == 0;
        }

        return false;
    }

    static AnyValuePtr instantiateWithLiteralValue(const AnyValuePtr &value)
    {
        if(value->isLiteralInteger() || value->isLiteralFraction() || value->isLiteralFloat())
            return makeValue(ValueType(value->unwrapAsFloat64()));

        return nullptr;
    }

    template<typename SourceType>
    static void addUnsignedIntegerConstructor(AnyValuePtrList &ctors)
    {
        typedef ObjectPtr<SourceType> SourceTypePtr;

        ctors.push_back(makeIntrinsicConstructor<PrimitiveFloatPtr (TypePtr, SourceTypePtr)> ("integer.conversion.unsigned.to-float", +[](const TypePtr &, const SourceTypePtr &value){
            return makeValue(ValueType(value->value));
        }, MethodFlags::Pure | MethodFlags::Explicit));
    }

    template<typename SourceType>
    static void addSignedIntegerConstructor(AnyValuePtrList &ctors)
    {
        typedef ObjectPtr<SourceType> SourceTypePtr;

        ctors.push_back(makeIntrinsicConstructor<PrimitiveFloatPtr (TypePtr, SourceTypePtr)> ("integer.conversion.signed.to-float", +[](const TypePtr &, const SourceTypePtr &value){
            return makeValue(ValueType(value->value));
        }, MethodFlags::Pure | MethodFlags::Explicit));
    }

    template<typename SourceType>
    static void addFloatConstructor(AnyValuePtrList &ctors)
    {
        typedef ObjectPtr<SourceType> SourceTypePtr;
        typedef typename SourceType::ValueType SourceValueType;

        static constexpr auto SourceValueTypeSize = sizeof(SourceValueType);
        static constexpr auto DestinationValueTypeSize = sizeof(ValueType);

        if(SourceValueTypeSize == DestinationValueTypeSize)
            return;

        ctors.push_back(makeIntrinsicConstructor<PrimitiveFloatPtr (TypePtr, SourceTypePtr)> (SourceValueTypeSize < DestinationValueTypeSize ? "float.conversion.extend" : "float.conversion.truncate", +[](const TypePtr &, const SourceTypePtr &value){
            return makeValue(ValueType(value->value));
        }, MethodFlags::Pure | MethodFlags::Explicit));
    }

    static AnyValuePtrList constructors()
    {
        auto ctors = AnyValuePtrList{
            makeConstructor<PrimitiveFloatPtr (TypePtr, LiteralIntegerPtr)> (+[](const TypePtr &, const LiteralIntegerPtr &value){
                return makeValue(ValueType(value->unwrapAsLargeInteger().asDouble()));
            }, MethodFlags::Explicit | MethodFlags::Pure),

            makeConstructor<PrimitiveFloatPtr (TypePtr, LiteralFractionPtr)> (+[](const TypePtr &, const LiteralFractionPtr &value){
                return makeValue(ValueType(value->unwrapAsFloat64()));
            }, MethodFlags::Explicit | MethodFlags::Pure),

            makeConstructor<PrimitiveFloatPtr (TypePtr, LiteralFloatPtr)> (+[](const TypePtr &, const LiteralFloatPtr &value){
                return makeValue(ValueType(value->unwrapAsFloat64()));
            }, MethodFlags::Explicit | MethodFlags::Pure),
        };

        addUnsignedIntegerConstructor<UInt8> (ctors);
        addUnsignedIntegerConstructor<UInt16> (ctors);
        addUnsignedIntegerConstructor<UInt32> (ctors);
        addUnsignedIntegerConstructor<UInt64> (ctors);

        addUnsignedIntegerConstructor<Char8> (ctors);
        addUnsignedIntegerConstructor<Char16> (ctors);
        addUnsignedIntegerConstructor<Char32> (ctors);

        addSignedIntegerConstructor<Int8> (ctors);
        addSignedIntegerConstructor<Int16> (ctors);
        addSignedIntegerConstructor<Int32> (ctors);
        addSignedIntegerConstructor<Int64> (ctors);

        addFloatConstructor<Float32> (ctors);
        addFloatConstructor<Float64> (ctors);
        return ctors;
    }

    static MethodCategories instanceMethods()
    {
        return MethodCategories{
            {"comparisons", {
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.equals", "=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.equals", "==", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value == b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.not-equals", "~=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.not-equals", "~~", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value != b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.less-than", "<", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value < b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.less-equals", "<=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value <= b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.greater-than", ">", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value > b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<Boolean8Ptr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.greater-equals", ">=", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return Boolean8::make(a->value >= b->value);
                }, MethodFlags::Pure),
            }},
            {"arithmetic", {
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.neg", "pre--", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(-v->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.neg", "negated", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(-v->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.reciprocal", "reciprocal", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(ValueType(1.0 / v->value));
                }, MethodFlags::Pure),

                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.sign", "sign", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(ValueType(v->value < 0 ? -1 : (v->value > 0 ? 1 : 0)));
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr)> ("float.abs", "abs", +[](const PrimitiveFloatPtr &v) {
                    return makeValue(v->value < 0 ? -v->value : v->value);
                }, MethodFlags::Pure),

                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.add", "+", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value + b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.sub", "-", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value - b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.mul", "*", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value * b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.div", "/", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value / b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.idiv", "//", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(trunc(a->value / b->value));
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.mod", "%", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(fmod(a->value, b->value));
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.mod", "\\\\", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(fmod(a->value, b->value));
                }, MethodFlags::Pure),

                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.min", "min:", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value <= b->value ? a->value : b->value);
                }, MethodFlags::Pure),
                makeIntrinsicMethodBinding<PrimitiveFloatPtr (PrimitiveFloatPtr, PrimitiveFloatPtr)> ("float.max", "max:", +[](const PrimitiveFloatPtr &a, const PrimitiveFloatPtr &b) {
                    return makeValue(a->value >= b->value ? a->value : b->value);
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
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("maxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::max()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("infinity", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::infinity()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("infinityOrMaxValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::infinity()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("negativeInfinity", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(-std::numeric_limits<ValueType>::infinity()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("negativeInfinityOrMinValue", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::infinity()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("nan", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::quiet_NaN()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("qNaN", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::quiet_NaN()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("sNaN", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(std::numeric_limits<ValueType>::signaling_NaN()));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("e", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(2.718281828459045)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("piReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.3183098861837907)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("pi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(3.141592653589793)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("twoPi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(6.283185307179586)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("twoPiReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.15915494309189535)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("threePi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(9.42477796076938)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("threePiReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.1061032953945969)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("halfPi", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(1.5707963267948966)));
                }, MethodFlags::Macro),
                makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("halfPiReciprocal", +[](const MacroInvocationContextPtr &context) {
                    return context->astBuilder->literal(makeValue(ValueType(0.6366197723675814)));
                }, MethodFlags::Macro),
            }}
        };
    }
};

bool PrimitiveFloatType::isPrimitiveFloatTypeValue() const
{
    return true;
}

AnyValuePtr PrimitiveFloatType::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitPrimitiveFloatType(selfFromThis());
}

MethodCategories Float32::__instanceMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float32>::instanceMethods();
}

MethodCategories Float32::__typeMacroMethods__()
{
    return IntrinsicPrimitiveFloatMethods<Float32>::typeMacroMethods();
}

AnyValuePtrList Float32::__constructors__()
{
    return IntrinsicPrimitiveFloatMethods<Float32>::constructors();
}

bool Float32::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveFloatMethods<Float32>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Float32::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveFloatMethods<Float32>::instantiateWithLiteralValue(value);
}

float Float32::unwrapAsFloat32() const
{
    return value;
}

double Float32::unwrapAsFloat64() const
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

AnyValuePtrList Float64::__constructors__()
{
    return IntrinsicPrimitiveFloatMethods<Float64>::constructors();
}

bool Float64::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveFloatMethods<Float64>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Float64::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveFloatMethods<Float64>::instantiateWithLiteralValue(value);
}

uint64_t Float64::__memoryAlignment__()
{
    return RuntimeContext::getActive()->getTargetDescription().doubleAlignment;
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

} // End of namespace Environment
} // End of namespace Sysmel