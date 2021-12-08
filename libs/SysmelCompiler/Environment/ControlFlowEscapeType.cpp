#include "Environment/ControlFlowEscapeType.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ControlFlowEscapeType> controlFlowEscapeTypeRegistration;

TypePtr Type::getControlFlowEscapeType()
{
    return ControlFlowEscapeType::__staticType__();
}

} // End of namespace Environment
} // End of namespace Sysmel