#include "sysmel/BootstrapEnvironment/UnionType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius