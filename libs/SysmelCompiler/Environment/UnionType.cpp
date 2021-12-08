#include "Environment/UnionType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<UnionTypeValue> UnionTypeValueTypeRegistration;

bool UnionType::isUnionType() const
{
    return true;
}

bool UnionTypeValue::isUnionTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel