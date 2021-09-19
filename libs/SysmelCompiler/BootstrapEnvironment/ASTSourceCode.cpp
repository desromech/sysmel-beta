#include "sysmel/BootstrapEnvironment/ASTSourceCode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius