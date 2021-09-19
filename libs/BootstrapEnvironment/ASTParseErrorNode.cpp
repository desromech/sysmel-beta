#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTParseErrorNode> ASTParseErrorNodeTypeRegistration;

bool ASTParseErrorNode::isASTParseErrorNode() const
{
    return true;
}

SExpression ASTParseErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"parseError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius