#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSABasicBlock> SSABasicBlockTypeRegistration;

bool SSABasicBlock::isSSABasicBlock() const
{
    return true;
}

AnyValuePtr SSABasicBlock::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitBasicBlock(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius