#include "sysmel/BootstrapEnvironment/ASTSourceCodePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSourceCodePosition> ASTSourceCodePositionTypeRegistration;

bool ASTSourceCodePosition::isASTSourceCodePosition() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius