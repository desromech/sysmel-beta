#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSACodeRegion> SSACodeRegionTypeRegistration;

bool SSACodeRegion::isSSACodeRegion() const
{
    return true;
}

AnyValuePtr SSACodeRegion::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCodeRegion(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius