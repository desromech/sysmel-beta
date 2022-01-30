#include "Environment/ArrayType.hpp"
#include "Environment/AggregateTypeArrayLayout.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTMakeAggregateNode.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/LiteralValueVisitor.hpp"
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

bool ArrayType::hasTrivialAssignCopyingFrom()
{
    return elementType->hasTrivialAssignCopyingFrom();
}

bool ArrayType::hasTrivialAssignMovingFrom()
{
    return elementType->hasTrivialAssignMovingFrom();
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

std::string ArrayType::getQualifiedName() const
{
    if(size == 0)
        return elementType->getQualifiedName() + "[]";
    
    std::ostringstream out;
    out << elementType->getQualifiedName();
    out << "[" << size << ']';
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

AggregateTypeValuePtr ArrayType::makeRawValueInstance()
{
    return basicMakeObject<ArrayTypeValue> ();
}

AnyValuePtr ArrayType::basicNewValue()
{
    auto array = basicMakeObject<ArrayTypeValue> ();
    array->type = selfFromThis();
    array->slots.reserve(size);
    for(size_t i = 0; i < size; ++i)
        array->slots.push_back(elementType->basicNewValue());
    return array;
}

void ArrayType::addSpecializedInstanceMethods()
{
    auto receiverType = asReceiverType();
    auto constReceiverType = asConstReceiverType();

    auto elementRefType = elementType->ref();
    auto elementConstRefType = elementType->withConst()->ref();

    /*auto accessElementSignedIndex = [](const ReferenceTypeValuePtr &self, const AnyValuePtr &index) {
        auto indexValue = index->unwrapAsInt64();
        sysmelAssert(self->baseValue && self->baseValue->isArrayTypeValue());
        auto array = self->baseValue.staticAs<ArrayTypeValue> ();
        auto elementType = array->getType().staticAs<ArrayType>()->elementType;

        auto offset = indexValue * elementType->getAlignedMemorySize();
        auto resultRefType = elementType->refForMemberOfReceiverWithType(self->getType());
        return array->getReferenceToSlotWithType(indexValue, offset, resultRefType);
    };*/

    auto accessElementUnsignedIndex = [](const ReferenceTypeValuePtr &self, const AnyValuePtr &index) {
        auto indexValue = index->unwrapAsUInt64();
        sysmelAssert(self->baseValue && self->baseValue->isArrayTypeValue());
        auto array = self->baseValue.staticAs<ArrayTypeValue> ();
        auto elementType = array->getType().staticAs<ArrayType>()->elementType;

        auto offset = indexValue * elementType->getAlignedMemorySize();
        auto resultRefType = elementType->refForMemberOfReceiverWithType(self->getType());
        return array->getReferenceToSlotWithType(indexValue, offset, resultRefType);
    };

    addMethodCategories(MethodCategories{
        {"accessing", {
            // []
            //makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr, AnyValuePtr)> ("array.element", "[]", receiverType, elementRefType, {Type::getIntPointerType()}, accessElementSignedIndex, MethodFlags::Pure),
            //makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr, AnyValuePtr)> ("array.element", "[]", constReceiverType, elementConstRefType, {Type::getIntPointerType()}, accessElementSignedIndex, MethodFlags::Pure),

            makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr, AnyValuePtr)> ("array.element", "[]", receiverType, elementRefType, {Type::getUIntPointerType()}, accessElementUnsignedIndex, MethodFlags::Pure),
            makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr, AnyValuePtr)> ("array.element", "[]", constReceiverType, elementConstRefType, {Type::getUIntPointerType()}, accessElementUnsignedIndex, MethodFlags::Pure),
        }},
    });
}

ASTNodePtr ArrayType::analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(size != 0 && arguments.size() != size)
        return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Cannot construct {0} with only {1} elements.", {printString(), castToString(arguments.size())}));

    auto result = basicMakeObject<ASTMakeAggregateNode> ();
    result->sourcePosition = node->sourcePosition;
    result->aggregateType = make(elementType, arguments.size())->asASTNodeRequiredInPosition(node->sourcePosition);
    result->elements = arguments;
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(result);
}

void ArrayType::buildLayout()
{
    auto arrayLayout = basicMakeObject<AggregateTypeArrayLayout> ();
    layout = arrayLayout;
    arrayLayout->setElementTypeAndSize(elementType, size);
}

MethodCategories ArrayTypeValue::__typeMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<TypePtr (TypePtr)> ("elementType", [](const TypePtr &type) -> TypePtr {
                return type->isArrayType() ? type.staticAs<ArrayType> ()->elementType : Type::getVoidType();
            }, MethodFlags::Pure),

            makeMethodBinding<uint64_t (TypePtr)> ("size", [](const TypePtr &type) -> uint64_t{
                return type->isArrayType() ? type.staticAs<ArrayType> ()->size : 0;
            }, MethodFlags::Pure),
        }}
    };
}

MethodCategories ArrayTypeValue::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("size", [](const MacroInvocationContextPtr &context) {
                auto decayedSelfType = context->selfType->asDecayedType();
                uint64_t size = decayedSelfType->isArrayType() ? decayedSelfType.staticAs<ArrayType> ()->size : 0;
                return context->astBuilder->literal(wrapValue(size));
            }, MethodFlags::Macro),
        }}
    };
}

bool ArrayTypeValue::isArrayTypeValue() const
{
    return true;
}

AnyValuePtr ArrayTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitArrayTypeValue(selfFromThis());
}

TypePtr ArrayTypeValue::getType() const
{
    return type;
}

std::string ArrayTypeValue::printString() const
{
    std::ostringstream out;
    out << type->printString();
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

SExpression ArrayTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(slots.size());
    for(auto &el : slots)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"array"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel