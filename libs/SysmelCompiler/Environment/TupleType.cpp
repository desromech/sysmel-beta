#include "Environment/TupleType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/FunctionType.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/AggregateTypeSequentialLayout.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTSlotAccessNode.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/StringUtilities.hpp"
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
        auto undecoratedType = elementType->asDecayedType();
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
    sysmelAssert(!elementTypes.empty());

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

std::string TupleType::getQualifiedName() const
{
    sysmelAssert(!elementTypes.empty());

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
    auto &slotTypes = getLayout().staticAs<AggregateTypeSequentialLayout> ()->getSlotTypes();

    auto tuple = basicMakeObject<TupleTypeValue> ();
    tuple->type = selfFromThis();
    tuple->slots.reserve(slotTypes.size());
    for(auto & slotType : slotTypes)
        tuple->slots.push_back(slotType->basicNewValue());
    return tuple;
}

AnyValuePtr TupleType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitTupleType(selfFromThis());
}

AggregateTypeValuePtr TupleType::makeRawValueInstance()
{
    return basicMakeObject<TupleTypeValue> ();
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
        elementSlotIndices.push_back(layout->addSlotWithType(type));
    layout->finishGroup();
}

MethodCategories TupleTypeValue::__typeMethods__()
{
    return MethodCategories{
        {"pattern matching", {
            makeMethodBinding<bool (const TypePtr &)> ("isSequencePatternType", +[](const TypePtr &){
                return true;
            }, MethodFlags::Pure),

            makeMethodBinding<uint64_t (const TypePtr &)> ("sequencePatternTypeMinSize", +[](const TypePtr &tupleType){
                return tupleType.staticAs<TupleType>()->elementTypes.size();
            }, MethodFlags::Pure),
            makeMethodBinding<uint64_t (const TypePtr &)> ("sequencePatternTypeMaxSize", +[](const TypePtr &tupleType){
                return tupleType.staticAs<TupleType>()->elementTypes.size();
            }, MethodFlags::Pure),
            makeMethodBinding<TypePtr (const TypePtr &, uint64_t)> ("sequencePatternTypeAt:", +[](const TypePtr &tupleType, uint64_t i){
                return i < tupleType.staticAs<TupleType>()->elementTypes.size() ? tupleType.staticAs<TupleType>()->elementTypes[i] : nullptr;
            }, MethodFlags::Pure),
        }}
    };
}

MethodCategories TupleTypeValue::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("__sequencePatternAt__:", [=](const MacroInvocationContextPtr &macroContext, ASTNodePtr index) -> ASTNodePtr {
               return macroContext->astBuilder->sendToWithArguments(macroContext->astBuilder->literalSymbol("[]"), macroContext->receiverNode, {index});
            }, MethodFlags::Macro),

            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, uint64_t)> ("[]", [=](const MacroInvocationContextPtr &macroContext, int64_t slotIndex) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                sysmelAssert(decayedType->isTupleType());
                
                auto tupleType = decayedType.staticAs<TupleType> ();
                if(slotIndex < 0 || size_t(slotIndex) >= tupleType->elementTypes.size())
                    return macroContext->astBuilder->semanticError(formatString("Tuple element index {0} is outside of bounds.", {castToString(slotIndex)}));

                auto elementType = tupleType->elementTypes[slotIndex];
                auto referenceType = elementType->refForMemberOfReceiverWithType(macroContext->selfType);
                return macroContext->astBuilder->slotAccess(macroContext->receiverNode, slotIndex, referenceType, true);
            }, MethodFlags::Macro)
        }},

        {"pattern matching", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("__sequencePatternSize__", [=](const MacroInvocationContextPtr &macroContext) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                sysmelAssert(decayedType->isTupleType());

                return macroContext->astBuilder->literal(wrapValue(uint64_t(decayedType.staticAs<TupleType> ()->elementTypes.size())));
            }, MethodFlags::Macro),
        }},
    };
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
    sysmelAssert(!slots.empty());

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