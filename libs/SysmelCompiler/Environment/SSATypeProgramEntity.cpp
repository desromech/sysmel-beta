#include "Environment/SSATypeProgramEntity.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSATypeProgramEntity> SSATypeProgramEntityTypeRegistration;

bool SSATypeProgramEntity::isSSATypeProgramEntity() const
{
    return true;
}

AnyValuePtr SSATypeProgramEntity::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitTypeProgramEntity(selfFromThis());
}

TypePtr SSATypeProgramEntity::getValueType() const
{
    return value->getType();
}

const TypePtr &SSATypeProgramEntity::getValue() const
{
    return value;
}

void SSATypeProgramEntity::setValue(const TypePtr &newValue)
{
    value = newValue;
}

SExpression SSATypeProgramEntity::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"type"}},
        value->asSExpression(),
    }};
}

SExpression SSATypeProgramEntity::asFullDefinitionSExpression() const
{
    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"type"}},
        value->asSExpression(),
        childrenSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel