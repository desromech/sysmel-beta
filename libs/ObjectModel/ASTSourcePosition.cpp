#include "sysmel/ObjectModel/ASTSourcePosition.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTSourcePosition> ASTSourcePositionTypeRegistration;

bool ASTSourcePosition::isASTSourcePosition() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius