#ifndef SYSMEL_ENVIRONMENT_CONTINUE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_CONTINUE_TYPE_HPP
#pragma once

#include "ControlFlowEscapeType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ContinueType);

/**
 * I am literal number value.
 */
class SYSMEL_COMPILER_LIB_EXPORT ContinueType : public SubtypeOf<ControlFlowEscapeType, ContinueType>
{
public:
    static constexpr char const __typeName__[] = "ContinueType";
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CONTINUE_TYPE_HPP