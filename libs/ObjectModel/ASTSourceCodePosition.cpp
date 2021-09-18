#include "sysmel/ObjectModel/ASTSourceCodePosition.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTSourceCodePosition> ASTSourceCodePositionTypeRegistration;

bool ASTSourceCodePosition::isASTSourceCodePosition() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius