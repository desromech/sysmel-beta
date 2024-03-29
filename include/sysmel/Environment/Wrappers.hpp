#ifndef SYSMEL_ENVIRONMENT_WRAPPERS_HPP
#define SYSMEL_ENVIRONMENT_WRAPPERS_HPP
#pragma once

#include "AnyValue.hpp"
#include "CannotUnwrap.hpp"
#include <utility>

namespace Sysmel
{
namespace Environment
{

//=============================================================================
// Wrapper type for
//=============================================================================

struct UIntPointerValue
{
    uint64_t value;
};

struct IntPointerValue
{
    int64_t value;
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<UIntPointerValue> 
{
    static TypePtr apply();
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<IntPointerValue> 
{
    static TypePtr apply();
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<bool>
{
    static TypePtr apply();
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<void>
{
    static TypePtr apply();
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<LargeInteger> 
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
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<char32_t> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<char> : WrapperTypeFor<char32_t> {};

template<>
struct WrapperTypeFor<char16_t> : WrapperTypeFor<char32_t> {};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<Fraction> 
{
    static TypePtr apply();
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<double> 
{
    static TypePtr apply();
};

template<>
struct WrapperTypeFor<float> : WrapperTypeFor<double> {};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<std::string> 
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
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeFor<AnyValuePtrList>
{
    static TypePtr apply();
};

template<typename T>
struct WrapperTypeFor<ObjectPtr<T>> : std::enable_if<std::is_base_of<AnyValue, T>::value>
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
struct SYSMEL_COMPILER_LIB_EXPORT WrapperTypeForReturning<Fraction>
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
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<bool>
{
    static AnyValuePtr apply(bool value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<uint32_t>
{
    static AnyValuePtr apply(uint32_t value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<int32_t>
{
    static AnyValuePtr apply(int32_t value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<uint64_t>
{
    static AnyValuePtr apply(uint64_t value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<int64_t>
{
    static AnyValuePtr apply(int64_t value);
};

struct LargeInteger;

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<LargeInteger>
{
    static AnyValuePtr apply(const LargeInteger &value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<char32_t>
{
    static AnyValuePtr apply(char32_t value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<Fraction> 
{
    static AnyValuePtr apply(const Fraction &value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<double>
{
    static AnyValuePtr apply(double value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<std::string>
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
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<AnyValuePtrList>
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
struct WrapValue<ObjectPtr<T>>
    : std::enable_if<std::is_base_of<AnyValue, T>::value, WrapValue<AnyValuePtr>>::type {};

template<typename T>
struct WrapValue<T&> : WrapValue<T> {};

template<typename T>
struct WrapValue<const T&> : WrapValue<T> {};

template<typename T>
struct WrapValue<T&&> : WrapValue<T> {};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<UIntPointerValue>
{
    static AnyValuePtr apply(UIntPointerValue value);
};

template<>
struct SYSMEL_COMPILER_LIB_EXPORT WrapValue<IntPointerValue>
{
    static AnyValuePtr apply(IntPointerValue value);
};

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

template<typename T>
struct UnwrapValue<std::vector<T>>
{
    static std::vector<T> apply(const AnyValuePtr &value)
    {
        const auto &elementList = value->unwrapAsArray();
        std::vector<T> result;
        result.reserve(elementList.size());
        for(auto &el : elementList)
            result.push_back(UnwrapValue<T>::apply(el));
        return result;
    }
};

struct UnwrapValueDummyBase {};

template<typename T>
struct UnwrapValue<ObjectPtr<T>> : std::enable_if<std::is_base_of<AnyValue, T>::value, UnwrapValueDummyBase>::type
{
    static ObjectPtr<T> apply(const AnyValuePtr &value)
    {
        auto castedValue = dynamicObjectCast<T> (value);
        if(!castedValue)
            signalNew<CannotUnwrap> ();
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
            signalNew<CannotUnwrap> ();
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
            signalNew<CannotUnwrap> ();
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

} // End of namespace Environment
} // End of namespace Sysmel
#endif //SYSMEL_ENVIRONMENT_WRAPPERS_HPP
