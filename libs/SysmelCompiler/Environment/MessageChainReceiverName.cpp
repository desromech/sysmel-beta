#include "Environment/MessageChainReceiverName.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<MessageChainReceiverName> MessageChainReceiverNameTypeRegistration;

std::string MessageChainReceiverName::printString() const
{
    return "MessageChainReceiver at " + sourcePosition->printString();
}

SExpression MessageChainReceiverName::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"messageChainReceiver"}},
        sourcePosition->asSExpression(),
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel