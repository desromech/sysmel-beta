#include "Environment/ArrayType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/StringUtilities.hpp"
namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ArrayTypeValue> ArrayTypeValueTypeRegistration;

ArrayTypePtr ArrayType::make(const TypePtr &elementType, uint64_t size)
{
    auto undecoratedElementType = elementType->asUndecoratedType();

    auto &cache = RuntimeContext::getActive()->arrayTypeCache;
    auto it = cache.find({undecoratedElementType, size});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<ArrayType> ();
    result->setSupertypeAndImplicitMetaType(ArrayTypeValue::__staticType__());
    result->elementType = undecoratedElementType;
    result->size = size;
    cache.insert({{undecoratedElementType, size}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool ArrayType::isArrayType() const
{
    return true;
}

AnyValuePtr ArrayType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitArrayType(selfFromThis());
}

bool ArrayType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool ArrayType::isNullableType() const
{
    return false;
}

bool ArrayType::isImmutableType()
{
    return elementType->isImmutableType();
}

bool ArrayType::hasTrivialInitialization()
{
    return elementType->hasTrivialInitialization();
}

bool ArrayType::hasTrivialInitializationCopyingFrom()
{
    return elementType->hasTrivialInitializationCopyingFrom();
}

bool ArrayType::hasTrivialInitializationMovingFrom()
{
    return elementType->hasTrivialInitializationMovingFrom();
}

bool ArrayType::hasTrivialFinalization()
{
    return elementType->hasTrivialFinalization();
}

bool ArrayType::hasTrivialCopyingFrom()
{
    return elementType->hasTrivialCopyingFrom();
}

bool ArrayType::hasTrivialMovingFrom()
{
    return elementType->hasTrivialMovingFrom();
}

uint64_t ArrayType::getMemorySize()
{
    if(size == 0)
        return size;
    
    return elementType->getAlignedMemorySize() * size;
}

uint64_t ArrayType::getMemoryAlignment()
{
    return elementType->getMemoryAlignment();
}

std::string ArrayType::printString() const
{
    if(size == 0)
        return elementType->printString() + " array";
    
    std::ostringstream out;
    out << '(';
    out << elementType->printString();
    out << " array: " << size << ')';
    return out.str();
}

SExpression ArrayType::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"arrayType"}},
        elementType->asSExpression(),
        LargeInteger{size}
    }};
}

AnyValuePtr ArrayType::basicNewValue()
{
    auto tuple = basicMakeObject<ArrayTypeValue> ();
    tuple->type = selfFromThis();
    tuple->elements.reserve(size);
    for(size_t i = 0; i < size; ++i)
        tuple->elements.push_back(elementType->basicNewValue());
    return tuple;
}

void ArrayType::addSpecializedInstanceMethods()
{
}

bool ArrayTypeValue::isArrayTypeValue() const
{
    return true;
}

TypePtr ArrayTypeValue::getType() const
{
    return type;
}

std::string ArrayTypeValue::printString() const
{
    assert(!elements.empty());

    std::ostringstream out;
    out << type->printString();
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

SExpression ArrayTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(elements.size());
    for(auto &el : elements)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"array"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel