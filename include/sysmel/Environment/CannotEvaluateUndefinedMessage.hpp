#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_UNDEFINED_MESSAGE_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_UNDEFINED_MESSAGE_HPP
#pragma once

#include "CannotEvaluateMessage.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Basic error
 */
class SYSMEL_COMPILER_LIB_EXPORT CannotEvaluateUndefinedMessage : public SubtypeOf<CannotEvaluateMessage, CannotEvaluateUndefinedMessage>
{
public:
    static constexpr char const __typeName__[] = "CannotEvaluateUndefinedMessage";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CANNOT_EVALUATE_UNDEFINED_MESSAGE_HPP