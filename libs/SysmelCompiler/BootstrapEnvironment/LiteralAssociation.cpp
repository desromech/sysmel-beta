#include "sysmel/BootstrapEnvironment/LiteralAssociation.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapFieldVariable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralAssociation> literalArrayRegistration;

LiteralAssociationPtr LiteralAssociation::makeWith(const AnyValuePtr &key, const AnyValuePtr &value)
{
    auto result = basicMakeObject<LiteralAssociation> ();
    result->key = key;
    result->value = value;
    return result;
}

FieldVariablePtrList LiteralAssociation::__fieldVariables__()
{
    return FieldVariablePtrList{
        makeBootstrapFieldVariable("key", &SelfType::key),
        makeBootstrapFieldVariable("value", &SelfType::value),
    };
}

MethodCategories LiteralAssociation::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            // =
            makeMethodBinding("key", &SelfType::getKey, MethodFlags::Pure),
            makeMethodBinding("value", &SelfType::getValue, MethodFlags::Pure),
        }},
    };
}

bool LiteralAssociation::isLiteralAssociation() const
{
    return true;
}

std::string LiteralAssociation::printString() const
{
    return validAnyValue(key)->printString() + " -> " + validAnyValue(value)->printString();
}

SExpression LiteralAssociation::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"association"}},
        key ? key->asSExpression() : nullptr,
        value ? value->asSExpression() : nullptr
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius