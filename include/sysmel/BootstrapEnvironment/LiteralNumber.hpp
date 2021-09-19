#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NUMBER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NUMBER_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class LiteralNumber;
typedef std::shared_ptr<LiteralNumber> LiteralNumberPtr;


/**
 * I am literal number value.
 */
class LiteralNumber : public SubtypeOf<LiteralValue, LiteralNumber>
{
public:
    static constexpr char const __typeName__[] = "LiteralNumber";

    static MethodCategories __instanceMethods__();

    virtual bool isLiteralNumber() const override;

    virtual double asFloat() const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_NUMBER_HPP