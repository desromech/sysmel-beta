#include "sysmel/ObjectModel/ASTSourceCode.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTSourceCode> ASTSourceCodeTypeRegistration;

bool ASTSourceCode::isASTSourceCode() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius