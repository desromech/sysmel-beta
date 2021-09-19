#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class LiteralBoolean;
typedef std::shared_ptr<LiteralBoolean> LiteralBooleanPtr;


/**
 * I am literal number value.
 */
class LiteralBoolean : public SubtypeOf<LiteralValue, LiteralBoolean>
{
public:
    static constexpr char const __typeName__[] = "LiteralBoolean";

    virtual bool isLiteralBoolean() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_BOOLEAN_HPP