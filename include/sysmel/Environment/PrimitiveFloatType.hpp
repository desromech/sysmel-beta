#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_FLOAT_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_FLOAT_TYPE_HPP
#pragma once

#include "PrimitiveNumberType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveFloatType);

/**
 * I am a primitive floating point type.
 */
class PrimitiveFloatType : public SubtypeOf<PrimitiveNumberType, PrimitiveFloatType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveFloatType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveFloatType";

    virtual bool isPrimitiveFloatTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
};

/**
 * I am an IEEE 754 16-bits floating point value.
 */
class Float16 : public SubtypeOf<PrimitiveFloatType, Float16>
{
public:
    static constexpr char const __typeName__[] = "Float16";
    static constexpr char const __sysmelTypeName__[] = "Float16";
    static constexpr uint64_t __memorySize__ = 2;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 2;
    }

    uint16_t value = 0;
};

/**
 * I am an IEEE 754 32-bits floating point value.
 */
class Float32 : public SubtypeOf<PrimitiveFloatType, Float32>
{
public:
    typedef float ValueType;

    static constexpr char const __typeName__[] = "Float32";
    static constexpr char const __sysmelTypeName__[] = "Float32";
    
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 4;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 4;
    }

    virtual float unwrapAsFloat32() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    ValueType value = 0;
};

/**
 * I am an IEEE 754 64-bits floating point value.
 */
class Float64 : public SubtypeOf<PrimitiveFloatType, Float64>
{
public:
    typedef double ValueType;

    static constexpr char const __typeName__[] = "Float64";
    static constexpr char const __sysmelTypeName__[] = "Float64";
    
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 8;
    static uint64_t __memoryAlignment__();

    virtual double unwrapAsFloat64() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    ValueType value = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_FLOAT_TYPE_HPP