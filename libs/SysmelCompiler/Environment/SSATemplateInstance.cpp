#include "Environment/SSATemplateInstance.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/TemplateInstance.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

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

SSAProgramEntityPtr SSATemplateInstance::getMainTemplateInstanceChild() const
{
    return children.empty() ? nullptr : children.front();
}

SExpression SSATemplateInstance::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"templateInstance"}},
    }};
}

std::string SSATemplateInstance::getInstanceNameWithArguments()
{
    std::ostringstream out;
    out << getParent()->getValidNameStringIncludingTemplateName();
    out << '(';
    bool isFirst = true;
    for(auto & arg : arguments)
    {
        if(isFirst)
            isFirst = false;
        else
            out << ", ";
        out << validAnyValue(arg)->printString();
    }
    out << ')';
    return out.str();
}

SExpression SSATemplateInstance::asFullDefinitionSExpression() const
{
    SExpressionList argsSExpr;
    argsSExpr.elements.reserve(arguments.size());
    for(auto &arg : arguments)
        argsSExpr.elements.push_back(arg->asFullDefinitionSExpression());

    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"templateInstance"}},
        argsSExpr,
        childrenSExpr
    }};
}

AnyValuePtrList &SSATemplateInstance::getArguments()
{
    return arguments;
}

void SSATemplateInstance::setArguments(const AnyValuePtrList &newArguments)
{
    arguments = newArguments;
}

} // End of namespace Environment
} // End of namespace Sysmel