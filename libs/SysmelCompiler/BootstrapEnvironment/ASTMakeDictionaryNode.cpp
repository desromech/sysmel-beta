#include "sysmel/BootstrapEnvironment/ASTMakeDictionaryNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMakeDictionaryNode> ASTMakeDictionaryNodeTypeRegistration;

bool ASTMakeDictionaryNode::isASTMakeDictionaryNode() const
{
    return true;
}

SExpression ASTMakeDictionaryNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(2 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"dict"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius