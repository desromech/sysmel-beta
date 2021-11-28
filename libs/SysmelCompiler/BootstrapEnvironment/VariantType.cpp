#include "sysmel/BootstrapEnvironment/VariantType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <unordered_set>
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

    auto result = std::make_shared<VariantType> ();
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
    auto variant = std::make_shared<VariantTypeValue> ();
    variant->type = shared_from_this();
    variant->wrappedElement = elementTypes.front()->basicNewValue();
    return variant;
}

TypePtr VariantType::appendTypeMakingVariant(const TypePtr &nextType)
{
    auto newElementTypes = elementTypes;
    if(nextType->isVariantType())
    {
        auto &nextTypes = std::static_pointer_cast<VariantType> (nextType)->elementTypes;
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius