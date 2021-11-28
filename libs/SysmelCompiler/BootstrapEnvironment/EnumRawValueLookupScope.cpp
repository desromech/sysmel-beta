#include "sysmel/BootstrapEnvironment/EnumRawValueLookupScope.hpp"
#include "sysmel/BootstrapEnvironment/EnumType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<EnumRawValueLookupScope> EnumRawValueLookupScopeRegistration;

AnyValuePtr EnumRawValueLookupScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    return enumType->lookupRawValue(symbol);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius