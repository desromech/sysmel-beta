#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ParseError.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTParseErrorNode> ASTParseErrorNodeTypeRegistration;

bool ASTParseErrorNode::isASTParseErrorNode() const
{
    return true;
}

AnyValuePtr ASTParseErrorNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitParseErrorNode(selfFromThis());
}

SExpression ASTParseErrorNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"parseError"}},
        sourcePosition->asSExpression(),
        errorMessage}};
}

CompilationErrorPtr ASTParseErrorNode::asCompilationError()
{
    auto result = basicMakeObject<ParseError> ();
    result->parseErrorNode = selfFromThis();
    return result;
}
} // End of namespace Environment
} // End of namespace Sysmel