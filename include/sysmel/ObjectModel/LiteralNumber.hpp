#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NUMBER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NUMBER_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am literal number value.
 */
class LiteralNumber : public SubtypeOf<LiteralValue, LiteralNumber>
{
public:
    static constexpr char const __typeName__[] = "LiteralNumber";

    virtual bool isLiteralNumber() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NUMBER_HPP