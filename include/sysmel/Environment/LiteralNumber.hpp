#ifndef SYSMEL_ENVIRONMENT_LITERAL_NUMBER_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_NUMBER_HPP
#pragma once

#include "LiteralValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralNumber);


/**
 * I am literal number value.
 */
class LiteralNumber : public SubtypeOf<LiteralValue, LiteralNumber>
{
public:
    static constexpr char const __typeName__[] = "LiteralNumber";
    static constexpr char const __sysmelTypeName__[] = "LiteralNumber";

    static MethodCategories __instanceMethods__();

    virtual bool isLiteralNumber() const override;

    virtual double asFloat() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_NUMBER_HPP