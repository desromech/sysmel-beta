#include "Environment/VariantType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/AggregateTypeVariantLayout.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTSlotAccessNode.hpp"
#include "Environment/ASTMakeVariantNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTProgramEntityExtensionNode.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/StringUtilities.hpp"
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
        auto undecoratedElementType = elementType->asDecayedType();
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
    sysmelAssert(!elementTypes.empty());

    auto &cache = RuntimeContext::getActive()->variantTypeCache;
    auto it = cache.find(elementTypes);
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<VariantType> ();
    result->setSupertypeAndImplicitMetaType(VariantTypeValue::__staticType__());
    result->elementTypes = elementTypes;
    for(size_t i = 0; i < elementTypes.size(); ++i)
        result->typeToSelectorMap.insert({elementTypes[i], i});
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

std::string VariantType::getQualifiedName() const
{
    sysmelAssert(!elementTypes.empty());

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

AggregateTypeValuePtr VariantType::makeRawValueInstance()
{
    return basicMakeObject<VariantTypeValue> ();
}

AnyValuePtr VariantType::basicNewValue()
{
    return makeWithValueAndSelector(elementTypes.front()->basicNewValue(), 0);
}

AnyValuePtr VariantType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitVariantType(selfFromThis());
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

bool VariantType::includesType(const TypePtr &typeToTest)
{
    return typeToSelectorMap.find(typeToTest->asDecayedType()) != typeToSelectorMap.end();
}

void VariantType::addSpecializedInstanceMethods()
{
}

ASTNodePtr VariantType::analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(arguments.size() == 1)
    {
        auto result = basicMakeObject<ASTMakeVariantNode> ();
        result->sourcePosition = node->sourcePosition;
        result->value = arguments[0];
        result->variantType = asASTNodeRequiredInPosition(node->sourcePosition);
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(result);
    }

    return SuperType::analyzeFallbackValueConstructionWithArguments(node, arguments, semanticAnalyzer);
}

std::optional<uint64_t> VariantType::findTypeSelectorIndexFor(const TypePtr &expectedType)
{
    auto it = typeToSelectorMap.find(expectedType);
    if(it != typeToSelectorMap.end())
        return it->second;
    return std::nullopt;
}

VariantTypeValuePtr VariantType::makeWithValueAndSelector(const AnyValuePtr &value, uint64_t selectorIndex)
{
    auto variantLayout = getLayout().staticAs<AggregateTypeVariantLayout> ();

    auto result = basicMakeObject<VariantTypeValue> ();
    result->type = selfFromThis();
    result->slots.resize(getLayout()->getSlotCount());
    result->slots[0] = variantLayout->getDataTypeIndexType()->instantiatedWithLiteralValue(wrapValue(selectorIndex));
    result->slots[variantLayout->getElementMemorySlotIndex()] = value;
    return result;
}

void VariantType::buildLayout()
{
    layout = basicMakeObject<AggregateTypeVariantLayout>();
    layout->beginGroup();
    for(auto &type : elementTypes)
        layout->addSlotWithType(type);
    layout->finishGroup();
}

MethodCategories VariantTypeValue::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("typeSelector", [=](const MacroInvocationContextPtr &macroContext) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                sysmelAssert(decayedType->isVariantType());
                
                auto variantType = decayedType.staticAs<VariantType> ();
                auto elementType = variantType->getLayout().staticAs<AggregateTypeVariantLayout> ()->getDataTypeIndexType();
                auto referenceType = elementType->refForMemberOfReceiverWithType(macroContext->selfType);

                return macroContext->astBuilder->slotAccess(macroContext->receiverNode, 0, referenceType, false);
            }, MethodFlags::Macro),

            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, TypePtr)> ("get:", [=](const MacroInvocationContextPtr &macroContext, const TypePtr &accessedType) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                sysmelAssert(decayedType->isVariantType());

                auto variantType = decayedType.staticAs<VariantType> ();

                auto decayedAccessedType = accessedType->asDecayedType();
                auto expectedTypeSelector = variantType->findTypeSelectorIndexFor(decayedAccessedType);
                if(!expectedTypeSelector.has_value())
                    return macroContext->astBuilder->semanticError(formatString("Variant does not contain requested type {0}.", {accessedType->printString()}));

                auto layout = variantType->getLayout().staticAs<AggregateTypeVariantLayout> ();
                auto slotIndex = layout->getElementMemorySlotIndex();
                
                auto selectorReferenceType = layout->getDataTypeIndexType()->refForMemberOfReceiverWithType(macroContext->selfType);
                auto referenceType = decayedAccessedType->refForMemberOfReceiverWithType(macroContext->selfType);

                return macroContext->astBuilder->variantSlotAccess(macroContext->receiverNode, slotIndex, referenceType,
                    0, selectorReferenceType, expectedTypeSelector.value(), false);
            }, MethodFlags::Macro)

        }},
    };
}

MethodCategories VariantTypeValue::__typeMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("extendAllWith:", [=](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &body) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                sysmelAssert(decayedType->isMetaType() && decayedType->getInstanceType()->isVariantType());
                auto variantType = decayedType->getInstanceType().staticAs<VariantType> ();
                
                ASTNodePtrList perTypeExtensions;
                perTypeExtensions.reserve(variantType->elementTypes.size());
                for(auto &alternative : variantType->elementTypes)
                    perTypeExtensions.push_back(macroContext->astBuilder->programEntityExtension(alternative->asASTNodeRequiredInPosition(macroContext->astBuilder->sourcePosition), body));

                return macroContext->astBuilder->sequence({}, perTypeExtensions);
            }, MethodFlags::Macro),
        }},
    };
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
    out << getType()->printString() << '(' << getWrappedElement()->printString() << ')';
    return out.str();
}

SExpression VariantTypeValue::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"variant"}},
        type->asSExpression(),
        getWrappedElement()->asSExpression()
    }};
}

const AnyValuePtr &VariantTypeValue::getTypeSelector() const
{
    return slots[0];
}

const AnyValuePtr &VariantTypeValue::getWrappedElement() const
{
    auto variantLayout = type->getLayout().staticAs<AggregateTypeVariantLayout> ();
    return slots[variantLayout->getElementMemorySlotIndex()];
}

} // End of namespace Environment
} // End of namespace Sysmel