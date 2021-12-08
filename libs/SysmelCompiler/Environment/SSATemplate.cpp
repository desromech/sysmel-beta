#include "Environment/SSATemplate.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Template.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel