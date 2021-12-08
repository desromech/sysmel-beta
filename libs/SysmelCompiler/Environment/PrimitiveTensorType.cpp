#include "Environment/PrimitiveTensorType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveTensorType> PrimitiveTensorTypeTypeRegistration;


bool PrimitiveTensorType::isPrimitiveTensorTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel