#include "Environment/PrimitiveNumberType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveNumberType> PrimitiveNumberTypeTypeRegistration;


bool PrimitiveNumberType::isPrimitiveNumberTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel