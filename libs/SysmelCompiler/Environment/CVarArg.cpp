#include "Environment/CVarArg.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<CVarArg> CVarArgTypeRegistration;

bool CVarArg::isCVarArg() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel