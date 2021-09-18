#ifndef SYSMEL_COMPILER_OBJECT_MODEL_WRAPPERS_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_WRAPPERS_HPP
#pragma once

#include "AnyValue.hpp"
#include "Error.hpp"
#include <utility>

namespace SysmelMoebius
{
namespace ObjectModel
{

//=============================================================================
// Wrapper type for
//=============================================================================

template<>
struct WrapperTypeFor<bool>
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<LargeInteger> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<uint8_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<int8_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<uint16_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<int16_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<uint32_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<int32_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<uint64_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<int64_t> : WrapperTypeFor<LargeInteger> {};

template<>
struct WrapperTypeFor<char32_t> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<char> : WrapperTypeFor<char32_t> {};

template<>
struct WrapperTypeFor<char16_t> : WrapperTypeFor<char32_t> {};

template<>
struct WrapperTypeFor<Fraction> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<double> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<float> : WrapperTypeFor<double> {};

template<>
struct WrapperTypeFor<std::string> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<const char *> : WrapperTypeFor<std::string> {};

template<size_t N>
struct WrapperTypeFor<const char (&)[N]> : WrapperTypeFor<std::string> {};

template<>
struct WrapperTypeFor<AnyValuePtr>
{
    static TypePtr apply()
    {
        return AnyValue::__staticType__();
    }
};

template<>
struct WrapperTypeFor<AnyValuePtrList>
{
    static TypePtr apply();
};

template<typename T>
struct WrapperTypeFor<std::shared_ptr<T>> : std::enable_if<std::is_base_of<AnyValue, T>::value>
{
    static TypePtr apply()
    {
        return T::__staticType__();
    }
};

template<typename T>
struct WrapperTypeFor<T*> : std::enable_if<std::is_base_of<AnyValue, T>::value>
{
    static TypePtr apply()
    {
        return T::__staticType__();
    }
};

template<typename T>
struct WrapperTypeFor<const T*> : std::enable_if<std::is_base_of<AnyValue, T>::value>
{
    static TypePtr apply()
    {
        return T::__staticType__();
    }
};

template<typename T>
struct WrapperTypeFor<std::vector<T>> : WrapperTypeFor<std::vector<AnyValuePtr>> {};

template<typename T>
struct WrapperTypeFor<T&> : WrapperTypeFor<T> {};

template<typename T>
struct WrapperTypeFor<const T&> : WrapperTypeFor<T> {};

template<typename T>
struct WrapperTypeFor<T&&> : WrapperTypeFor<T> {};

template<typename T>
TypePtr wrapperTypeFor()
{
    return WrapperTypeFor<T>::apply();
}

template<typename T>
struct WrapperTypeForReturning : WrapperTypeFor<T> {};

template<>
struct WrapperTypeForReturning<Fraction>
{
    static TypePtr apply();
};

template<typename T>
TypePtr wrapperTypeForReturning()
{
    return WrapperTypeForReturning<T>::apply();
}

//=============================================================================
// Wrap Value
//=============================================================================
template<>
struct WrapValue<bool>
{
    static AnyValuePtr apply(bool value);
};

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
struct WrapValue<Fraction> 
{
    static AnyValuePtr apply(const Fraction &value);
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

template<>
struct WrapValue<AnyValuePtr>
{
    static AnyValuePtr apply(const AnyValuePtr &value)
    {
        return value;
    }
};

template<>
struct WrapValue<AnyValuePtrList>
{
    static AnyValuePtr apply(const AnyValuePtrList &content);
};

template<typename T>
struct WrapValue<std::vector<T>>
{
    static AnyValuePtr apply(const std::vector<T> &content)
    {
        AnyValuePtrList objectContent;
        objectContent.reserve(content.size());
        for(const auto &element : content)
            objectContent.push_back(WrapValue<T>::apply(element));

        return WrapValue<AnyValuePtrList>::apply(objectContent);
    }
};



template<typename T>
struct WrapValue<std::shared_ptr<T>>
    : std::enable_if<std::is_base_of<AnyValue, T>::value, WrapValue<AnyValuePtr>>::type {};

template<typename T>
struct WrapValue<T&> : WrapValue<T> {};

template<typename T>
struct WrapValue<const T&> : WrapValue<T> {};

template<typename T>
struct WrapValue<T&&> : WrapValue<T> {};

template<typename T>
AnyValuePtr wrapValue(T &&value)
{
    return WrapValue<T>::apply(std::forward<T> (value));
}

//=============================================================================
// Unwrap Value
//=============================================================================

template<>
struct UnwrapValue<bool>
{
    static bool apply(const AnyValuePtr &value)
    {
        return value->unwrapAsBoolean();
    }
};

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
struct UnwrapValue<Fraction>
{
    static Fraction apply(const AnyValuePtr &value)
    {
        return value->unwrapAsFraction();
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

template<>
struct UnwrapValue<AnyValuePtr>
{
    static AnyValuePtr apply(const AnyValuePtr &value)
    {
        return value;
    }
};

struct UnwrapValueDummyBase {};

template<typename T>
struct UnwrapValue<std::shared_ptr<T>> : std::enable_if<std::is_base_of<AnyValue, T>::value, UnwrapValueDummyBase>::type
{
    static std::shared_ptr<T> apply(const AnyValuePtr &value)
    {
        auto castedValue = std::dynamic_pointer_cast<T> (value);
        if(!castedValue)
            throw CannotUnwrap();
        return castedValue;
    }
};

template<typename T>
struct UnwrapValue<T*> : std::enable_if<std::is_base_of<AnyValue, T>::value, UnwrapValueDummyBase>::type
{
    static T* apply(const AnyValuePtr &value)
    {
        auto castedValue = dynamic_cast<T*> (value.get());
        if(!castedValue)
            throw CannotUnwrap();
        return castedValue;
    }
};

template<typename T>
struct UnwrapValue<const T*> : std::enable_if<std::is_base_of<AnyValue, T>::value, UnwrapValueDummyBase>::type
{
    static const T* apply(const AnyValuePtr &value)
    {
        auto castedValue = dynamic_cast<const T*> (value.get());
        if(!castedValue)
            throw CannotUnwrap();
        return castedValue;
    }
};

template<typename T>
struct UnwrapValue<const T &> : UnwrapValue<T> {};

template<typename T>
auto unwrapValue(const AnyValuePtr &value)
{
    return UnwrapValue<T>::apply(value);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
#endif //SYSMEL_COMPILER_OBJECT_MODEL_WRAPPERS_HPP
