#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<Type> typeTypeRegistration;

TypePtr Type::getSuperType()
{
    return TypePtr();
}

AnyValuePtr Type::lookupSelector(const AnyValuePtr &selector)
{
    return AnyValuePtr();
}

AnyValuePtr Type::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    auto method = lookupSelector(selector);
    if(method)
        return method->runWithArgumentsIn(selector, arguments, receiver);

    throw MessageNotUnderstood("Message " + selector->printString() + " is not understood by " + receiver->printString() + ".");
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius