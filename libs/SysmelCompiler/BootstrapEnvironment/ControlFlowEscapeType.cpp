#include "sysmel/BootstrapEnvironment/ControlFlowEscapeType.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ControlFlowEscapeType> controlFlowEscapeTypeRegistration;

TypePtr Type::getControlFlowEscapeType()
{
    return ControlFlowEscapeType::__staticType__();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius