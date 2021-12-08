#include "Environment/ReceiverMemberLookupScope.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ReceiverMemberLookupScope> ReceiverMemberLookupScopeRegistration;

AnyValuePtr ReceiverMemberLookupScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    auto found = receiverType->lookupLocalSymbolFromScope(symbol, selfFromThis());
    return found ? found->asMemberBoundWithReceiverVariable(receiverVariable) : nullptr;
}

} // End of namespace Environment
} // End of namespace Sysmel