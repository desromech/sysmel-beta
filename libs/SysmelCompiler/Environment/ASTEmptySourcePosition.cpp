#include "Environment/ASTEmptySourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTEmptySourcePosition> ASTEmptySourcePositionTypeRegistration;

ASTSourcePositionPtr ASTSourcePosition::empty()
{
    static auto singleton = basicMakeGlobalSingletonObject<ASTEmptySourcePosition> ();
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

} // End of namespace Environment
} // End of namespace Sysmel