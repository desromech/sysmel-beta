#include "Environment/ASTSourceCode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSourceCode> ASTSourceCodeTypeRegistration;

bool ASTSourceCode::isASTSourceCode() const
{
    return true;
}

SExpression ASTSourceCode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"sourceCode"}},
        name
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel