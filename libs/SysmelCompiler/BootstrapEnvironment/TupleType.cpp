#include "sysmel/BootstrapEnvironment/TupleType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<TupleTypeValue> TupleTypeValueTypeRegistration;

TypePtr TupleType::make(const TypePtrList &elementTypes)
{
    TypePtrList normalizedTypes;
    normalizedTypes.reserve(elementTypes.size());

    for(auto &elementType : elementTypes)
    {
        if(elementType->isVoidType())
            continue;

        normalizedTypes.push_back(elementType);
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

    auto result = std::make_shared<TupleType> ();
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
    auto tuple = std::make_shared<TupleTypeValue> ();
    tuple->type = shared_from_this();
    tuple->elements.reserve(elementTypes.size());
    for(auto & elType : elementTypes)
        tuple->elements.push_back(elType->basicNewValue());
    return tuple;
}

TupleTypeValuePtr TupleType::makeWithElements(const AnyValuePtrList &elements)
{
    auto tuple = std::make_shared<TupleTypeValue> ();
    tuple->type = shared_from_this();
    tuple->elements = elements;
    return tuple;
}

TypePtr TupleType::asTupleType()
{
    return shared_from_this();
}

TypePtr TupleType::appendTypeMakingTuple(const TypePtr &nextType)
{
    auto newElementTypes = elementTypes;
    if(nextType->isTupleType())
    {
        auto &nextTypes = std::static_pointer_cast<TupleType> (nextType)->elementTypes;
        newElementTypes.insert(newElementTypes.end(), nextTypes.begin(), nextTypes.end());
    }
    else
    {
        newElementTypes.push_back(nextType);
    }

    return TupleType::make(newElementTypes);
}

void TupleType::addSpecializedInstanceMethods()
{
}

bool TupleTypeValue::isTupleTypeValue() const
{
    return true;
}

TypePtr TupleTypeValue::getType() const
{
    return type;
}

std::string TupleTypeValue::printString() const
{
    assert(!elements.empty());

    std::ostringstream out;
    out << '(';
    bool first = true;
    for(auto &el : elements)
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
    elementsSExpr.elements.reserve(elements.size());
    for(auto &el : elements)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"tuple"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius