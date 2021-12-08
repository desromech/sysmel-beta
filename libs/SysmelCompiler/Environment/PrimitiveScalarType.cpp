#include "Environment/PrimitiveScalarType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveScalarType> PrimitiveScalarTypeTypeRegistration;


bool PrimitiveScalarType::isPrimitiveScalarTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel