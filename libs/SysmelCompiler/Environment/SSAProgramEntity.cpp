#include "Environment/SSAProgramEntity.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel