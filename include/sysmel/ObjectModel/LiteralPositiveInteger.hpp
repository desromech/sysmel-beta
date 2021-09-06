#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_POSITIVE_INTEGER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_POSITIVE_INTEGER_HPP
#pragma once

#include "LiteralInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a literal positive integer value.
 */
class LiteralPositiveInteger : public SubtypeOf<LiteralInteger, LiteralPositiveInteger>
{
public:
    static constexpr char const __typeName__[] = "LiteralPositiveInteger";

    virtual bool isLiteralPositiveInteger() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_POSITIVE_INTEGER_HPP