#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_CHARACTER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_CHARACTER_HPP
#pragma once

#include "LiteralPositiveInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralCharacter : public SubtypeOf<LiteralPositiveInteger, LiteralCharacter>
{
public:
    static constexpr char const __typeName__[] = "LiteralCharacter";

    virtual bool isLiteralCharacter() const override;
    virtual std::string asString() const override;
    virtual std::string printString() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_CHARACTER_HPP