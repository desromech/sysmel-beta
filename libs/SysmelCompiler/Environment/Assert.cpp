#include "Environment/Assert.hpp"
#include <stdlib.h>
#include <iostream>

namespace Sysmel
{
namespace Environment
{

[[noreturn]] void assertFailureWithMessage(const char *message)
{
    std::cerr << message << std::endl;
    abort();
}

} // End of namespace Environment
} // End of namespace Sysmel