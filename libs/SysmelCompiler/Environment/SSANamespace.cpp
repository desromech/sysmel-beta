#include "Environment/SSANamespace.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Namespace.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSANamespace> SSANamespaceTypeRegistration;

bool SSANamespace::isSSANamespace() const
{
    return true;
}

AnyValuePtr SSANamespace::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitNamespace(selfFromThis());
}

TypePtr SSANamespace::getValueType() const
{
    return Namespace::__staticType__();
}

SExpression SSANamespace::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"namespace"}},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression SSANamespace::asFullDefinitionSExpression() const
{
    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"namespace"}},
        validAnyValue(name)->asSExpression(),
        childrenSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel