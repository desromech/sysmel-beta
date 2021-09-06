#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NEGATIVE_INTEGER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NEGATIVE_INTEGER_HPP
#pragma once

#include "LiteralInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a literal negative integer value.
 */
class LiteralNegativeInteger : public SubtypeOf<LiteralInteger, LiteralNegativeInteger>
{
public:
    static constexpr char const __typeName__[] = "LiteralNegativeInteger";

    virtual bool isLiteralNegativeInteger() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NEGATIVE_INTEGER_HPP