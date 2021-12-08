#include "Environment/LiteralAssociation.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapFieldVariable.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel