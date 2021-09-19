#include "sysmel/BootstrapEnvironment/ASTSourceCodePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourceCode.hpp"
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

SExpression ASTSourceCodePosition::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"position"}},
        sourceCode->asSExpression(),
        LargeInteger{startPosition},
        LargeInteger{endPosition},

        LargeInteger{startLine},
        LargeInteger{startColumn},

        LargeInteger{endLine},
        LargeInteger{endColumn},
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius