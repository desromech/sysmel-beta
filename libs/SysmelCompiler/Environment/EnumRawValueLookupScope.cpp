#include "Environment/EnumRawValueLookupScope.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<EnumRawValueLookupScope> EnumRawValueLookupScopeRegistration;

AnyValuePtr EnumRawValueLookupScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    return enumType->lookupRawValue(symbol);
}

} // End of namespace Environment
} // End of namespace Sysmel