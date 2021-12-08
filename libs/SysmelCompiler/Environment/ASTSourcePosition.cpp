#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSourcePosition> ASTSourcePositionTypeRegistration;

bool ASTSourcePosition::isASTSourcePosition() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel