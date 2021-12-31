#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/Error.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/Utilities.hpp"
#include <algorithm>
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveVectorTypeValue> PrimitiveVectorTypeValueTypeRegistration;

TypePtr PrimitiveVectorType::make(const TypePtr &elementType, uint32_t elements)
{
    auto undecoratedElementType = elementType->asUndecoratedType();
    if(elements <= 1)
        return elementType;
    
    auto &cache = RuntimeContext::getActive()->primitiveVectorTypeCache;
    auto it = cache.find({undecoratedElementType, elements});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<PrimitiveVectorType> ();
    result->setSupertypeAndImplicitMetaType(PrimitiveVectorTypeValue::__staticType__());
    result->elementType = undecoratedElementType;
    result->elements = elements;
    cache.insert({{undecoratedElementType, elements}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

AnyValuePtr PrimitiveVectorType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitPrimitiveVectorType(selfFromThis());
}

bool PrimitiveVectorType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool PrimitiveVectorType::isPrimitiveVectorType() const
{
    return true;
}

bool PrimitiveVectorType::isNullableType() const
{
    return false;
}

bool PrimitiveVectorType::hasTrivialInitialization()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialInitializationCopyingFrom()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialInitializationMovingFrom()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialFinalization()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialAssignCopyingFrom()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialAssignMovingFrom()
{
    return true;
}

uint64_t PrimitiveVectorType::getMemorySize()
{
    return elementType->getAlignedMemorySize() * elements;
}

uint64_t PrimitiveVectorType::getMemoryAlignment()
{
    auto alignmentFactor = asHighPowerOfTwo(elements);
    return elementType->getMemoryAlignment()*alignmentFactor;
}

std::string PrimitiveVectorType::printString() const
{
    if(name && !name->isAnonymousNameSymbol())
        return SuperType::printString();

    std::ostringstream out;
    out << '(';
    out << elementType->printString();
    out << " primitiveVectorWithElements: " << elements << ')';
    return out.str();
}

SExpression PrimitiveVectorType::asSExpression() const
{
    if(name && !name->isAnonymousNameSymbol())
        return SuperType::asSExpression();

    return SExpressionList{{
        SExpressionIdentifier{{"primitiveVectorType"}},
        elementType->asSExpression(),
        LargeInteger{elements}
    }};
}

AnyValuePtr PrimitiveVectorType::basicNewValue()
{
    auto vector = basicMakeObject<PrimitiveVectorTypeValue> ();
    vector->type = selfFromThis();
    vector->elements.reserve(elements);
    for(size_t i = 0; i < elements; ++i)
        vector->elements.push_back(elementType->basicNewValue());
    return vector;
}

void PrimitiveVectorType::addSpecializedInstanceMethods()
{
    auto accessElement = [](const PrimitiveVectorTypeValuePtr &self, const AnyValuePtr &index) {
        auto indexValue = index->unwrapAsInt64();
        if(indexValue < 0 || size_t(indexValue) > self->elements.size())
            signalNewWithMessage<Error> ("Vector element index is out of range.");

        return self->elements[indexValue];
    };

    addMethodCategories(MethodCategories{
        {"accessing", {
            // []
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, AnyValuePtr)> ("vector.element", "[]", selfFromThis(), elementType, {Type::getIntPointerType()}, accessElement, MethodFlags::Pure),
        }},
    });
}


bool PrimitiveVectorTypeValue::isPrimitiveVectorTypeValue() const
{
    return true;
}

AnyValuePtr PrimitiveVectorTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitPrimitiveVectorTypeValue(selfFromThis());
}

TypePtr PrimitiveVectorTypeValue::getType() const
{
    return type;
}

std::string PrimitiveVectorTypeValue::printString() const
{
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

SExpression PrimitiveVectorTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(elements.size());
    for(auto &el : elements)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"primitiveVector"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel
