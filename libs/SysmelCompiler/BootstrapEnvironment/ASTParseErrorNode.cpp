#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
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

AnyValuePtr ASTParseErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitParseErrorNode(shared_from_this());
}

SExpression ASTParseErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"parseError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

ParseErrorPtr ASTParseErrorNode::asParseError()
{
    auto result = std::make_shared<ParseError> ();
    result->parseErrorNode = shared_from_this();
    return result;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius