#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAConstantLiteralValue> SSAConstantLiteralValueTypeRegistration;

SSAConstantLiteralValuePtr SSAConstantLiteralValue::makeWith(const AnyValuePtr &value, const TypePtr &valueType)
{
    auto result = basicMakeObject<SSAConstantLiteralValue> ();
    result->value = value;
    result->valueType = valueType;
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

SExpression SSAConstantLiteralValue::asFullSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"constantLiteralValue"}},
        validAnyValue(value)->asSExpression(),
        valueType->asSExpression()
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
