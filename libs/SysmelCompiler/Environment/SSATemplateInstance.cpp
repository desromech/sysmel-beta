#include "Environment/SSATemplateInstance.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/TemplateInstance.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSATemplateInstance> SSATemplateInstanceTypeRegistration;

bool SSATemplateInstance::isSSATemplateInstance() const
{
    return true;
}

AnyValuePtr SSATemplateInstance::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitTemplateInstance(selfFromThis());
}

TypePtr SSATemplateInstance::getValueType() const
{
    return TemplateInstance::__staticType__();
}

SExpression SSATemplateInstance::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"templateInstance"}},
    }};
}

SExpression SSATemplateInstance::asFullDefinitionSExpression() const
{
    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"templateInstance"}},
        childrenSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel