#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_HPP
#pragma once

#include "Error.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT CannotEvaluateMessage : public SubtypeOf<Error, CannotEvaluateMessage>
{
public:
    static constexpr char const __typeName__[] = "CannotEvaluateMessage";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_MESSAGE_HPP
