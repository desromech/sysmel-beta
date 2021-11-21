#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_CHARACTER_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_CHARACTER_TYPE_HPP
#pragma once

#include "PrimitiveIntegerType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveCharacterType);

/**
 * I am a primitive character type.
 */
class PrimitiveCharacterType : public SubtypeOf<PrimitiveIntegerType, PrimitiveCharacterType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveCharacterType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveCharacterType";

    virtual bool isPrimitiveCharacterTypeValue() const override;
};

/**
 * I am a 8-bit character code unit.
 */
class Char8 : public SubtypeOf<PrimitiveCharacterType, Char8>
{
public:
    typedef char ValueType;

    static constexpr char const __typeName__[] = "Char8";
    static constexpr char const __sysmelTypeName__[] = "Char8";
    
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);

    virtual char unwrapAsChar8() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    char value = 0;
};

/**
 * I am a 16-bit character code unit.
 */
class Char16 : public SubtypeOf<PrimitiveCharacterType, Char16>
{
public:
    typedef char16_t ValueType;

    static constexpr char const __typeName__[] = "Char16";
    static constexpr char const __sysmelTypeName__[] = "Char16";
    
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);

    virtual char16_t unwrapAsChar16() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    char16_t value = 0;
};

/**
 * I am a 32-bit character code unit.
 */
class Char32 : public SubtypeOf<PrimitiveCharacterType, Char32>
{
public:
    typedef char32_t ValueType;

    static constexpr char const __typeName__[] = "Char32";
    static constexpr char const __sysmelTypeName__[] = "Char32";
    
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);

    virtual char32_t unwrapAsChar32() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    char32_t value = 0;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIMITIVE_CHARACTER_TYPE_HPP