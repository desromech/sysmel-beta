#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FLOAT_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FLOAT_HPP
#pragma once

#include "LiteralNumber.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralFloat : public SubtypeOf<LiteralNumber, LiteralFloat>
{
public:
    static constexpr char const __typeName__[] = "LiteralFloat";
    
    static MethodCategories __instanceMethods__();

    LiteralFloat(double ctorValue = 0.0)
        : value(ctorValue) {}

    virtual bool isLiteralFloat() const override;

    virtual std::string printString() const override;

    virtual float unwrapAsFloat32() const override;
    virtual double unwrapAsFloat64() const override;

    virtual double asFloat() const;
    
protected:
    double value;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FLOAT_HPP