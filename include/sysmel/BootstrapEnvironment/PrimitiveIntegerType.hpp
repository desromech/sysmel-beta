#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP
#pragma once

#include "PrimitiveNumberType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveIntegerType);

/**
 * I am a primitive integer type.
 */
class PrimitiveIntegerType : public SubtypeOf<PrimitiveNumberType, PrimitiveIntegerType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveIntegerType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveIntegerType";

    virtual bool isPrimitiveIntegerTypeValue() const override;
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

    int64_t value = 0;
};


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP