#include "sysmel/BootstrapEnvironment/SSANamespace.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius