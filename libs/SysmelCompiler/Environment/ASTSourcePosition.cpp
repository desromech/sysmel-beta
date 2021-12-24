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

std::string ASTSourcePosition::getSourceCodeFileName() const
{
    return "";
}

int ASTSourcePosition::getLine() const
{
    return 0;
}

int ASTSourcePosition::getColumn() const
{
    return 0;
}

} // End of namespace Environment
} // End of namespace Sysmel