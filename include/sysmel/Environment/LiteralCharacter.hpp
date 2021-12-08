#ifndef SYSMEL_ENVIRONMENT_LITERAL_CHARACTER_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_CHARACTER_HPP
#pragma once

#include "LiteralPositiveInteger.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralCharacter : public SubtypeOf<LiteralPositiveInteger, LiteralCharacter>
{
public:
    static constexpr char const __typeName__[] = "LiteralCharacter";
    static constexpr char const __sysmelTypeName__[] = "LiteralCharacter";

    static MethodCategories __instanceMethods__();
    
    virtual bool isLiteralCharacter() const override;
    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_CHARACTER_HPP