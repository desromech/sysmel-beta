#include "sysmel/BootstrapEnvironment/MessageChainReceiverName.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius