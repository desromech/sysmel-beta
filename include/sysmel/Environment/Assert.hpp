#ifndef SYSMEL_ENVIRONMENT_ASSERT_HPP
#define SYSMEL_ENVIRONMENT_ASSERT_HPP
#pragma once

#include "../DLLInterface.hpp"

namespace Sysmel
{
namespace Environment
{

[[noreturn]] SYSMEL_COMPILER_LIB_EXPORT void assertFailureWithMessage(const char *message);

#define sysmelSourceLineToString2(x) #x
#define sysmelSourceLineToString(x) sysmelSourceLineToString2(x)
#define sysmelAssert(expr) if(!(expr)) ::Sysmel::Environment::assertFailureWithMessage(__FILE__ ":" sysmelSourceLineToString(__LINE__) ": Assertion failure: " #expr)

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP