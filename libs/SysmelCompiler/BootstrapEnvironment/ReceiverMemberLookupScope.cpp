#include "sysmel/BootstrapEnvironment/ReceiverMemberLookupScope.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ReceiverMemberLookupScope> ReceiverMemberLookupScopeRegistration;

AnyValuePtr ReceiverMemberLookupScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    auto found = receiverType->lookupLocalSymbolFromScope(symbol, selfFromThis());
    return found ? found->asMemberBoundWithReceiverVariable(receiverVariable) : nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius