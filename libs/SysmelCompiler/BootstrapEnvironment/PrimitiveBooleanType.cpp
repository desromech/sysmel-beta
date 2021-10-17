#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveBooleanType> PrimitiveBooleanTypeTypeRegistration;
static BootstrapTypeRegistration<Boolean8> Boolean8TypeRegistration;


bool PrimitiveBooleanType::isPrimitiveBooleanTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius