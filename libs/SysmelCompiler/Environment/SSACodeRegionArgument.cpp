#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSACodeRegionArgument> SSACodeRegionArgumentTypeRegistration;

SSACodeRegionArgumentPtr SSACodeRegionArgument::make(const TypePtr &valueType, const TypePtr &sourceType)
{
    auto result = basicMakeObject<SSACodeRegionArgument> ();
    result->valueType = valueType;
    result->sourceType = sourceType;
    return result;
}

SSACodeRegionArgumentPtr SSACodeRegionArgument::makeReceiver(const TypePtr &valueType)
{
    auto result = basicMakeObject<SSACodeRegionArgument> ();
    result->valueType = valueType;
    result->sourceType = valueType;
    result->isReceiver_ = true;
    return result;
}

SSACodeRegionArgumentPtr SSACodeRegionArgument::makeResult(const TypePtr &valueType)
{
    auto result = basicMakeObject<SSACodeRegionArgument> ();
    result->valueType = valueType;
    result->sourceType = valueType;
    result->isResult_ = true;
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

const ASTSourcePositionPtr &SSACodeRegionArgument::getDeclarationPosition() const
{
    return declarationSourcePosition;
}

void SSACodeRegionArgument::setDefinitionPosition(const ASTSourcePositionPtr &position)
{
    definitionSourcePosition = position;
}

const ASTSourcePositionPtr &SSACodeRegionArgument::getDefinitionPosition() const
{
    return definitionSourcePosition;
}

TypePtr SSACodeRegionArgument::getValueType() const
{
    return valueType;
}

bool SSACodeRegionArgument::isLocalFinalizationRequired() const
{
    return localFinalizationRequired;
}

void SSACodeRegionArgument::markLocalFinalizationRequired()
{
    sysmelAssert(isResult_);
    localFinalizationRequired = true;
}

const TypePtr &SSACodeRegionArgument::getSourceType() const
{
    return sourceType;
}

void SSACodeRegionArgument::setSourceType(const TypePtr &newSourceType)
{
    sourceType = newSourceType;
}

const VariablePtr &SSACodeRegionArgument::getVariable() const
{
    return variable;
}

void SSACodeRegionArgument::setVariable(const VariablePtr &newVariable)
{
    variable = newVariable;
}

} // End of namespace Environment
} // End of namespace Sysmel