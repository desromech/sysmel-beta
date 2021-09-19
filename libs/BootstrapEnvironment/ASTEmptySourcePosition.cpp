#include "sysmel/BootstrapEnvironment/ASTEmptySourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTEmptySourcePosition> ASTEmptySourcePositionTypeRegistration;

ASTSourcePositionPtr ASTSourcePosition::empty()
{
    static auto singleton = std::make_shared<ASTEmptySourcePosition> ();
    return singleton;
}

bool ASTEmptySourcePosition::isASTEmptySourcePosition() const
{
    return true;
}

SExpression ASTEmptySourcePosition::asSExpression() const
{
    return SExpressionIdentifier{{"emptySourcePosition"}};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius