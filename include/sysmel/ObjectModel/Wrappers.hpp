#ifndef SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
#pragma once

#include "AnyValue.hpp"
#include <utility>

namespace SysmelMoebius
{
namespace ObjectModel
{

template<>
struct WrapValue<uint32_t>
{
    static AnyValuePtr apply(uint32_t value);
};

template<>
struct WrapValue<int32_t>
{
    static AnyValuePtr apply(int32_t value);
};

template<>
struct WrapValue<uint64_t>
{
    static AnyValuePtr apply(uint64_t value);
};

template<>
struct WrapValue<int64_t>
{
    static AnyValuePtr apply(int64_t value);
};

struct LargeInteger;

template<>
struct WrapValue<LargeInteger>
{
    static AnyValuePtr apply(const LargeInteger &value);
};

template<>
struct WrapValue<char32_t>
{
    static AnyValuePtr apply(char32_t value);
};

template<>
struct WrapValue<double>
{
    static AnyValuePtr apply(double value);
};

template<>
struct WrapValue<std::string>
{
    static AnyValuePtr apply(const std::string &value);
};

template<>
struct WrapValue<uint8_t> : WrapValue<uint32_t> {};

template<>
struct WrapValue<int8_t> : WrapValue<int32_t> {};

template<>
struct WrapValue<uint16_t> : WrapValue<uint32_t> {};

template<>
struct WrapValue<int16_t> : WrapValue<int32_t> {};

template<>
struct WrapValue<char> : WrapValue<char32_t> {};

template<>
struct WrapValue<char16_t> : WrapValue<char32_t> {};

template<>
struct WrapValue<float> : WrapValue<double> {};

template<>
struct WrapValue<const char *> : WrapValue<std::string> {};

template<size_t N>
struct WrapValue<const char (&)[N]> : WrapValue<std::string> {};

template<typename T>
AnyValuePtr wrapValue(T &&value)
{
    return WrapValue<T>::apply(std::forward<T> (value));
}

template<>
struct UnwrapValue<uint8_t>
{
    static uint8_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsUInt8();
    }
};

template<>
struct UnwrapValue<int8_t>
{
    static int8_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsInt8();
    }
};

template<>
struct UnwrapValue<uint16_t>
{
    static uint16_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsUInt16();
    }
};

template<>
struct UnwrapValue<int16_t>
{
    static int16_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsInt16();
    }
};

template<>
struct UnwrapValue<uint32_t>
{
    static uint32_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsUInt32();
    }
};

template<>
struct UnwrapValue<int32_t>
{
    static int32_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsInt32();
    }
};

template<>
struct UnwrapValue<uint64_t>
{
    static uint64_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsUInt64();
    }
};

template<>
struct UnwrapValue<int64_t>
{
    static int64_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsInt64();
    }
};

template<>
struct UnwrapValue<LargeInteger>
{
    static LargeInteger apply(const AnyValuePtr &value)
    {
        return value->unwrapAsLargeInteger();
    }
};

template<>
struct UnwrapValue<char>
{
    static char apply(const AnyValuePtr &value)
    {
        return value->unwrapAsChar8();
    }
};

template<>
struct UnwrapValue<char16_t>
{
    static char16_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsChar16();
    }
};

template<>
struct UnwrapValue<char32_t>
{
    static char32_t apply(const AnyValuePtr &value)
    {
        return value->unwrapAsChar32();
    }
};

template<>
struct UnwrapValue<float>
{
    static float apply(const AnyValuePtr &value)
    {
        return value->unwrapAsFloat32();
    }
};

template<>
struct UnwrapValue<double>
{
    static double apply(const AnyValuePtr &value)
    {
        return value->unwrapAsFloat64();
    }
};

template<>
struct UnwrapValue<std::string>
{
    static std::string apply(const AnyValuePtr &value)
    {
        return value->unwrapAsString();
    }
};

template<typename T>
T unwrapValue(const AnyValuePtr &value)
{
    return UnwrapValue<T>::apply(value);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
#endif //SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
