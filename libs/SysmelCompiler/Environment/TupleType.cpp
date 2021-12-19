#include "Environment/TupleType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/FunctionType.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/AggregateTypeSequentialLayout.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<TupleTypeValue> TupleTypeValueTypeRegistration;

TypePtr TupleType::make(const TypePtrList &elementTypes)
{
    TypePtrList normalizedTypes;
    normalizedTypes.reserve(elementTypes.size());

    for(auto &elementType : elementTypes)
    {
        auto undecoratedType = elementType->asUndecoratedType();
        if(undecoratedType->isVoidType())
            continue;

        normalizedTypes.push_back(undecoratedType);
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

TupleTypePtr TupleType::makeNormalized(const TypePtrList &elementTypes)
{
    assert(!elementTypes.empty());

    auto &cache = RuntimeContext::getActive()->tupleTypeCache;
    auto it = cache.find(elementTypes);
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<TupleType> ();
    result->setSupertypeAndImplicitMetaType(TupleTypeValue::__staticType__());
    result->elementTypes = elementTypes;
    cache.insert({elementTypes, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool TupleType::isTupleType() const
{
    return true;
}

bool TupleType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool TupleType::isNullableType() const
{
    return false;
}

std::string TupleType::printString() const
{
    assert(!elementTypes.empty());

    bool hasTupleTypeElement = false;
    for(auto &el : elementTypes)
    {
        if(el->isTupleType())
        {
            hasTupleTypeElement = true;
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
            out << (hasTupleTypeElement ? ", " : " & ");
        out << el->printString();
    }
    out << ')';
    if(hasTupleTypeElement)
        out << " tuple";

    return out.str();
}

SExpression TupleType::asSExpression() const
{
    SExpressionList elementTypesSExpr;
    elementTypesSExpr.elements.reserve(elementTypes.size());
    for(auto &el : elementTypes)
        elementTypesSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"tupleType"}},
        elementTypesSExpr
    }};
}

AnyValuePtr TupleType::basicNewValue()
{
    auto tuple = basicMakeObject<TupleTypeValue> ();
    tuple->type = selfFromThis();
    tuple->slots.reserve(elementTypes.size());
    for(auto & elType : elementTypes)
        tuple->slots.push_back(elType->basicNewValue());
    return tuple;
}

TupleTypeValuePtr TupleType::makeWithElements(const AnyValuePtrList &elements)
{
    auto tuple = basicMakeObject<TupleTypeValue> ();
    tuple->type = selfFromThis();
    tuple->slots = elements;
    return tuple;
}

TypePtr TupleType::asTupleType()
{
    return selfFromThis();
}

TypePtr TupleType::appendTypeMakingTuple(const TypePtr &nextType)
{
    auto newElementTypes = elementTypes;
    if(nextType->isTupleType())
    {
        auto &nextTypes = staticObjectCast<TupleType> (nextType)->elementTypes;
        newElementTypes.insert(newElementTypes.end(), nextTypes.begin(), nextTypes.end());
    }
    else
    {
        newElementTypes.push_back(nextType);
    }

    return TupleType::make(newElementTypes);
}

TypePtr TupleType::appendResultTypeMakingFunctionType(const TypePtr &nextType)
{
    return FunctionType::make(nextType, elementTypes);
}

void TupleType::addSpecializedInstanceMethods()
{
}

void TupleType::buildLayout()
{
    layout = basicMakeObject<AggregateTypeSequentialLayout>();
    layout->beginGroup();
    for(auto &type : elementTypes)
        layout->addSlotWithType(type);
    layout->finishGroup();
}

bool TupleTypeValue::isTupleTypeValue() const
{
    return true;
}

AnyValuePtr TupleTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitTupleTypeValue(selfFromThis());
}

TypePtr TupleTypeValue::getType() const
{
    return type;
}

std::string TupleTypeValue::printString() const
{
    assert(!slots.empty());

    std::ostringstream out;
    out << '(';
    bool first = true;
    for(auto &el : slots)
    {
        if(first)
            first = false;
        else
            out << ", ";
        out << el->printString();
    }
    out << ')';

    return out.str();
}

SExpression TupleTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(slots.size());
    for(auto &el : slots)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"tuple"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel