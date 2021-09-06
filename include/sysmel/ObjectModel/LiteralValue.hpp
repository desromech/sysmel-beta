#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_VALUE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_VALUE_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralValue : public SubtypeOf<AnyValue, LiteralValue>
{
public:
    static constexpr char const __typeName__[] = "LiteralValue";

    virtual bool isLiteralValue() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_VALUE_HPP