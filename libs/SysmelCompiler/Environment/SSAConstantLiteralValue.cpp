#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAConstantLiteralValue> SSAConstantLiteralValueTypeRegistration;

SSAConstantLiteralValuePtr SSAConstantLiteralValue::makeWith(const AnyValuePtr &value, const TypePtr &valueType, const ASTSourcePositionPtr &sourcePosition)
{
    auto result = basicMakeObject<SSAConstantLiteralValue> ();
    result->value = value;
    result->valueType = valueType;
    result->sourcePosition = sourcePosition;
    return result;
}

bool SSAConstantLiteralValue::isSSAConstantLiteralValue() const
{
    return true;
}

AnyValuePtr SSAConstantLiteralValue::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitConstantLiteralValue(selfFromThis());
}

SExpression SSAConstantLiteralValue::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"constantLiteralValue"}},
        validAnyValue(value)->asSExpression(),
        valueType->asSExpression(),
        sourcePosition ? sourcePosition->asSExpression() : nullptr,
    }};
}

TypePtr SSAConstantLiteralValue::getValueType() const
{
    return valueType;
}

const AnyValuePtr &SSAConstantLiteralValue::getValue() const
{
    return value;
}

} // End of namespace Environment
} // End of namespace Sysmel
