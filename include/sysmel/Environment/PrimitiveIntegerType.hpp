#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP
#pragma once

#include "PrimitiveNumberType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveIntegerType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(UInt8);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UInt16);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UInt32);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UInt64);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Int8);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Int16);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Int32);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Int64);

/**
 * I am a primitive integer type.
 */
class PrimitiveIntegerType : public SubtypeOf<PrimitiveNumberType, PrimitiveIntegerType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveIntegerType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveIntegerType";

    virtual bool isPrimitiveIntegerTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
};

/**
 * I am a 8-bit unsigned integer.
 */
class UInt8 : public SubtypeOf<PrimitiveIntegerType, UInt8>
{
public:
    typedef uint8_t ValueType;

    static constexpr char const __typeName__[] = "UInt8";
    static constexpr char const __sysmelTypeName__[] = "UInt8";
    
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 1;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 1;
    }

    virtual uint8_t unwrapAsUInt8() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    ValueType value = 0;
};

/**
 * I am a 16-bit unsigned integer.
 */
class UInt16 : public SubtypeOf<PrimitiveIntegerType, UInt16>
{
public:
    typedef uint16_t ValueType;

    static constexpr char const __typeName__[] = "UInt16";
    static constexpr char const __sysmelTypeName__[] = "UInt16";

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 2;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 2;
    }

    virtual uint16_t unwrapAsUInt16() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    uint16_t value = 0;
};

/**
 * I am a 32-bit unsigned integer.
 */
class UInt32 : public SubtypeOf<PrimitiveIntegerType, UInt32>
{
public:
    typedef uint32_t ValueType;

    static constexpr char const __typeName__[] = "UInt32";
    static constexpr char const __sysmelTypeName__[] = "UInt32";

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

    virtual uint32_t unwrapAsUInt32() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    uint32_t value = 0;
};

/**
 * I am a 64-bit unsigned integer.
 */
class UInt64 : public SubtypeOf<PrimitiveIntegerType, UInt64>
{
public:
    typedef uint64_t ValueType;

    static constexpr char const __typeName__[] = "UInt64";
    static constexpr char const __sysmelTypeName__[] = "UInt64";

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 8;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 8;
    }

    virtual uint64_t unwrapAsUInt64() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    uint64_t value = 0;
};

/**
 * I am a 8-bit signed integer.
 */
class Int8 : public SubtypeOf<PrimitiveIntegerType, Int8>
{
public:
    typedef int8_t ValueType;

    static constexpr char const __typeName__[] = "Int8";
    static constexpr char const __sysmelTypeName__[] = "Int8";

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 1;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 1;
    }

    virtual int8_t unwrapAsInt8() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    int8_t value = 0;
};

/**
 * I am a 16-bit signed integer.
 */
class Int16 : public SubtypeOf<PrimitiveIntegerType, Int16>
{
public:
    typedef int16_t ValueType;

    static constexpr char const __typeName__[] = "Int16";
    static constexpr char const __sysmelTypeName__[] = "Int16";

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 2;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 2;
    }

    virtual int16_t unwrapAsInt16() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    int16_t value = 0;
};

/**
 * I am a 32-bit signed integer.
 */
class Int32 : public SubtypeOf<PrimitiveIntegerType, Int32>
{
public:
    typedef int32_t ValueType;

    static constexpr char const __typeName__[] = "Int32";
    static constexpr char const __sysmelTypeName__[] = "Int32";

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

    virtual int32_t unwrapAsInt32() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    int32_t value = 0;
};

/**
 * I am a 64-bit signed integer.
 */
class Int64 : public SubtypeOf<PrimitiveIntegerType, Int64>
{
public:
    typedef int64_t ValueType;

    static constexpr char const __typeName__[] = "Int64";
    static constexpr char const __sysmelTypeName__[] = "Int64";

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 8;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 8;
    }

    virtual int64_t unwrapAsInt64() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    int64_t value = 0;
};


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP