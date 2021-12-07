#include "sysmel/BootstrapEnvironment/SSAProgramEntityWithChildren.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAProgramEntityWithChildren> SSAProgramEntityWithChildrenTypeRegistration;

void SSAProgramEntityWithChildren::addChild(const SSAProgramEntityPtr &child)
{
    children.push_back(child);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius