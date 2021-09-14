#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FRACTION_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FRACTION_HPP
#pragma once

#include "LiteralNumber.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralFraction : public SubtypeOf<LiteralNumber, LiteralFraction>
{
public:
    static constexpr char const __typeName__[] = "LiteralFraction";

    static MethodCategories __instanceMethods__();

    static LiteralNumberPtr makeFor(const Fraction &value);

    virtual bool isLiteralFraction() const override;
    virtual std::string printString() const override;

    virtual Fraction unwrapAsFraction() const override;
    virtual float unwrapAsFloat32() const override;
    virtual double unwrapAsFloat64() const override;

    const Fraction &getValue() const
    {
        return value;
    }

    virtual double asFloat() const override;

protected:
    Fraction value;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FRACTION_HPP