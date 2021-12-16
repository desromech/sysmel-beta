#include "Environment/VariantType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <unordered_set>
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<VariantTypeValue> VariantTypeValueTypeRegistration;

TypePtr VariantType::make(const TypePtrList &elementTypes)
{
    std::unordered_set<TypePtr> seenTypes;

    TypePtrList normalizedTypes;
    normalizedTypes.reserve(elementTypes.size());

    for(auto &elementType : elementTypes)
    {
        auto undecoratedElementType = elementType->asUndecoratedType();
        if(undecoratedElementType->isVoidType())
            continue;

        if(seenTypes.find(undecoratedElementType) != seenTypes.end())
            continue;

        seenTypes.insert(undecoratedElementType);
        normalizedTypes.push_back(undecoratedElementType);
    }

    switch(normalizedTypes.size())
    {
    case 0:
        return Type::getVoidType();
    case 1:
        return normalizedTypes[0];
    default:
        return makeNormalized(normalizedTypes);
    }
}

VariantTypePtr VariantType::makeNormalized(const TypePtrList &elementTypes)
{
    assert(!elementTypes.empty());

    auto &cache = RuntimeContext::getActive()->variantTypeCache;
    auto it = cache.find(elementTypes);
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<VariantType> ();
    result->setSupertypeAndImplicitMetaType(VariantTypeValue::__staticType__());
    result->elementTypes = elementTypes;
    cache.insert({elementTypes, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool VariantType::isVariantType() const
{
    return true;
}

bool VariantType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool VariantType::isNullableType() const
{
    return false;
}

std::string VariantType::printString() const
{
    assert(!elementTypes.empty());

    bool hasVariantTypeElement = false;
    for(auto &el : elementTypes)
    {
        if(el->isVariantType())
        {
            hasVariantTypeElement = true;
            break;
        }
    }

    std::ostringstream out;
    out << '(';
    bool first = true;
    for(auto &el : elementTypes)
    {
        if(first)
            first = false;
        else
            out << (hasVariantTypeElement ? ", " : " | ");
        out << el->printString();
    }
    out << ')';
    if(hasVariantTypeElement)
        out << " variant";

    return out.str();
}

SExpression VariantType::asSExpression() const
{
    SExpressionList elementTypesSExpr;
    elementTypesSExpr.elements.reserve(elementTypes.size());
    for(auto &el : elementTypes)
        elementTypesSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"variantType"}},
        elementTypesSExpr
    }};
}

AnyValuePtr VariantType::basicNewValue()
{
    auto variant = basicMakeObject<VariantTypeValue> ();
    variant->type = selfFromThis();
    variant->wrappedElement = elementTypes.front()->basicNewValue();
    return variant;
}

TypePtr VariantType::appendTypeMakingVariant(const TypePtr &nextType)
{
    auto newElementTypes = elementTypes;
    if(nextType->isVariantType())
    {
        auto &nextTypes = staticObjectCast<VariantType> (nextType)->elementTypes;
        newElementTypes.insert(newElementTypes.end(), nextTypes.begin(), nextTypes.end());
    }
    else
    {
        newElementTypes.push_back(nextType);
    }

    return VariantType::make(newElementTypes);
}

void VariantType::addSpecializedInstanceMethods()
{
}

bool VariantTypeValue::isVariantTypeValue() const
{
    return true;
}

AnyValuePtr VariantTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitVariantTypeValue(selfFromThis());
}

TypePtr VariantTypeValue::getType() const
{
    return type;
}

std::string VariantTypeValue::printString() const
{
    std::ostringstream out;
    out << getType()->printString() << '(' << wrappedElement->printString() << ')';
    return out.str();
}

SExpression VariantTypeValue::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"variant"}},
        type->asSExpression(),
        LargeInteger{typeIndex},
        wrappedElement->asSExpression()
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel