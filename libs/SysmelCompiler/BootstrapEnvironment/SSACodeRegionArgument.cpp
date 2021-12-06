#include "sysmel/BootstrapEnvironment/SSACodeRegionArgument.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSACodeRegionArgument> SSACodeRegionArgumentTypeRegistration;

SSACodeRegionArgumentPtr SSACodeRegionArgument::make(const TypePtr &valueType)
{
    auto result = basicMakeObject<SSACodeRegionArgument> ();
    result->valueType = valueType;
    return result;
}

bool SSACodeRegionArgument::isSSACodeRegionArgument() const
{
    return true;
}

AnyValuePtr SSACodeRegionArgument::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCodeRegionArgument(selfFromThis());
}

SExpression SSACodeRegionArgument::asFullDefinitionSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"argument"}},
        LargeInteger{localValueIndex},
        valueType->asSExpression(),
        declarationSourcePosition ? declarationSourcePosition->asSExpression() : nullptr,
        definitionSourcePosition ? definitionSourcePosition->asSExpression() : nullptr,
    }};
}

void SSACodeRegionArgument::setDeclarationPosition(const ASTSourcePositionPtr &position)
{
    declarationSourcePosition = position;
}

void SSACodeRegionArgument::setDefinitionPosition(const ASTSourcePositionPtr &position)
{
    definitionSourcePosition = position;
}

TypePtr SSACodeRegionArgument::getValueType() const
{
    return valueType;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius