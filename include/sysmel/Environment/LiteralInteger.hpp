#ifndef SYSMEL_ENVIRONMENT_LITERAL_INTEGER_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_INTEGER_HPP
#pragma once

#include "LiteralNumber.hpp"
#include "LargeInteger.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralInteger);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralCharacter);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class SYSMEL_COMPILER_LIB_EXPORT LiteralInteger : public SubtypeOf<LiteralNumber, LiteralInteger>
{
public:
    static constexpr char const __typeName__[] = "LiteralInteger";
    static constexpr char const __sysmelTypeName__[] = "LiteralInteger";

    static MethodCategories __instanceMethods__();

    static LiteralIntegerPtr makeFor(const LargeInteger &value);
    static LiteralIntegerPtr makeFor(LargeInteger &&value);
    static LiteralCharacterPtr makeForCharacter(char32_t value);

    virtual bool isLiteralInteger() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual std::string printString() const override;

    virtual uint8_t unwrapAsUInt8() const override;
    virtual int8_t unwrapAsInt8() const override;
    virtual uint16_t unwrapAsUInt16() const override;
    virtual int16_t unwrapAsInt16() const override;
    virtual uint32_t unwrapAsUInt32() const override;
    virtual int32_t unwrapAsInt32() const override;
    virtual uint64_t unwrapAsUInt64() const override;
    virtual int64_t unwrapAsInt64() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;

    virtual char unwrapAsChar8() const override;
    virtual char16_t unwrapAsChar16() const override;
    virtual char32_t unwrapAsChar32() const override;

    virtual Fraction unwrapAsFraction() const override;
    
    virtual float unwrapAsFloat32() const override;
    virtual double unwrapAsFloat64() const override;

    const LargeInteger &getValue() const
    {
        return value;
    }

    virtual double asFloat() const override;
    virtual SExpression asSExpression() const override;
    
protected:
    LargeInteger value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_INTEGER_HPP