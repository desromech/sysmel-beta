#ifndef SYSMEL_ENVIRONMENT_RETURN_TYPE_HPP
#define SYSMEL_ENVIRONMENT_RETURN_TYPE_HPP
#pragma once

#include "ControlFlowEscapeType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReturnType);

/**
 * I am literal number value.
 */
class ReturnType : public SubtypeOf<ControlFlowEscapeType, ReturnType>
{
public:
    static constexpr char const __typeName__[] = "ReturnType";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_RETURN_TYPE_HPP