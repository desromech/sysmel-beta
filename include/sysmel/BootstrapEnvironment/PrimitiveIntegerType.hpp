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
    static constexpr char const __typeName__[] = "UInt8";
    static constexpr char const __sysmelTypeName__[] = "UInt8";

    uint8_t value = 0;
};

/**
 * I am a 16-bit unsigned integer.
 */
class UInt16 : public SubtypeOf<PrimitiveIntegerType, UInt16>
{
public:
    static constexpr char const __typeName__[] = "UInt16";
    static constexpr char const __sysmelTypeName__[] = "UInt16";

    uint16_t value = 0;
};

/**
 * I am a 32-bit unsigned integer.
 */
class UInt32 : public SubtypeOf<PrimitiveIntegerType, UInt32>
{
public:
    static constexpr char const __typeName__[] = "UInt32";
    static constexpr char const __sysmelTypeName__[] = "UInt32";

    uint32_t value = 0;
};

/**
 * I am a 64-bit unsigned integer.
 */
class UInt64 : public SubtypeOf<PrimitiveIntegerType, UInt64>
{
public:
    static constexpr char const __typeName__[] = "UInt64";
    static constexpr char const __sysmelTypeName__[] = "UInt64";

    uint64_t value = 0;
};

/**
 * I am a 8-bit signed integer.
 */
class Int8 : public SubtypeOf<PrimitiveIntegerType, Int8>
{
public:
    static constexpr char const __typeName__[] = "Int8";
    static constexpr char const __sysmelTypeName__[] = "Int8";

    int8_t value = 0;
};

/**
 * I am a 16-bit signed integer.
 */
class Int16 : public SubtypeOf<PrimitiveIntegerType, Int16>
{
public:
    static constexpr char const __typeName__[] = "Int16";
    static constexpr char const __sysmelTypeName__[] = "Int16";

    int16_t value = 0;
};

/**
 * I am a 32-bit signed integer.
 */
class Int32 : public SubtypeOf<PrimitiveIntegerType, Int32>
{
public:
    static constexpr char const __typeName__[] = "Int32";
    static constexpr char const __sysmelTypeName__[] = "Int32";

    int32_t value = 0;
};

/**
 * I am a 64-bit signed integer.
 */
class Int64 : public SubtypeOf<PrimitiveIntegerType, Int64>
{
public:
    static constexpr char const __typeName__[] = "Int64";
    static constexpr char const __sysmelTypeName__[] = "Int64";

    int64_t value = 0;
};


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_INTEGER_TYPE_HPP