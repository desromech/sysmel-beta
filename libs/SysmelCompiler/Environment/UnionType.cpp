#include "Environment/UnionType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/AggregateTypeUnionLayout.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<UnionTypeValue> UnionTypeValueTypeRegistration;

bool UnionType::isUnionType() const
{
    return true;
}

AggregateTypeLayoutPtr UnionType::makeLayoutInstance()
{
    return basicMakeObject<AggregateTypeUnionLayout> ();
}

bool UnionTypeValue::isUnionTypeValue() const
{
    return true;
}

AnyValuePtr UnionTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitUnionTypeValue(selfFromThis());
}

SExpression UnionTypeValue::asSExpression() const
{
    // TODO: Base64 encode the data.
    return SExpressionList{{
        SExpressionIdentifier{{"union"}},
        type->asSExpression()}};
}

} // End of namespace Environment
} // End of namespace Sysmel