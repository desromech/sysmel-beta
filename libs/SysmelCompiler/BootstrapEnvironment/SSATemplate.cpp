#include "sysmel/BootstrapEnvironment/SSATemplate.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Template.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSATemplate> SSATemplateTypeRegistration;

bool SSATemplate::isSSATemplate() const
{
    return true;
}

AnyValuePtr SSATemplate::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitTemplate(selfFromThis());
}

TypePtr SSATemplate::getValueType() const
{
    return Template::__staticType__();
}

SExpression SSATemplate::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"template"}},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression SSATemplate::asFullDefinitionSExpression() const
{
    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"template"}},
        validAnyValue(name)->asSExpression(),
        childrenSExpr
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius