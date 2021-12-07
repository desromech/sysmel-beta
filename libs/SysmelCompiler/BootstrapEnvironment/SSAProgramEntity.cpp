#include "sysmel/BootstrapEnvironment/SSAProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAProgramEntity> SSAProgramEntityTypeRegistration;

bool SSAProgramEntity::isSSAProgramEntity() const
{
    return true;
}

AnyValuePtr SSAProgramEntity::getName() const
{
    return name;
}

void SSAProgramEntity::setName(const AnyValuePtr &newName)
{
    name = newName;
}

void SSAProgramEntity::addChild(const SSAProgramEntityPtr &)
{
    signalNew<UnsupportedOperation> ();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius