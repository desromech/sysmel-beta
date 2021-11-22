#include "sysmel/BootstrapEnvironment/ValueBox.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ValueBox> valueBoxTypeRegistration;

bool ValueBox::isValueBox() const
{
    return true;
}

AnyValuePtr ValueBox::copyAssignValue(const AnyValuePtr &newValue)
{
    value = newValue;
    return shared_from_this();
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
