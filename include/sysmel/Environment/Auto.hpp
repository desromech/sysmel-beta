#ifndef SYSMEL_ENVIRONMENT_AUTO_HPP
#define SYSMEL_ENVIRONMENT_AUTO_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Auto);

/**
 * I am literal number value.
 */
class Auto : public SubtypeOf<AnyValue, Auto>
{
public:
    static constexpr char const __typeName__[] = "Auto";
    static constexpr bool __isDynamicCompileTimeType__ = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AUTO_HPP