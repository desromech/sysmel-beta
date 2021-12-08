#include "Environment/SSAProgramEntityWithChildren.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAProgramEntityWithChildren> SSAProgramEntityWithChildrenTypeRegistration;

void SSAProgramEntityWithChildren::addChild(const SSAProgramEntityPtr &child)
{
    children.push_back(child);
}

} // End of namespace Environment
} // End of namespace Sysmel