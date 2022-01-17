#include "Environment/PrimitiveType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveType> PrimitiveTypeTypeRegistration;

bool PrimitiveType::isPrimitiveTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel