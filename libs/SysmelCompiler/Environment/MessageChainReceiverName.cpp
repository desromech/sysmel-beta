#include "Environment/MessageChainReceiverName.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<MessageChainReceiverName> MessageChainReceiverNameTypeRegistration;

bool MessageChainReceiverName::isHiddenNameSymbol() const
{
    return true;
}

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