#include "Environment/StructureType.hpp"
#include "Environment/Error.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/Error.hpp"
#include "Environment/AggregateTypeSequentialLayout.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTSlotAccessNode.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<StructureTypeValue> StructureTypeValueTypeRegistration;

bool StructureType::isStructureType() const
{
    return true;
}

AggregateTypeValuePtr StructureType::makeRawValueInstance()
{
    return basicMakeObject<StructureTypeValue> ();
}

AnyValuePtr StructureType::basicNewValue()
{
    auto sequentialLayout = getLayout().staticAs<AggregateTypeSequentialLayout> ();
    sysmelAssert(sequentialLayout);

    const auto &slotTypes = sequentialLayout->getSlotTypes();

    auto result = basicMakeObject<StructureTypeValue> ();
    result->type = selfFromThis();
    result->slots.reserve(slotTypes.size());

    for(const auto &slotType : slotTypes)
        result->slots.push_back(slotType->basicNewValue());

    return result;
}

AnyValuePtr StructureType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitStructureType(selfFromThis());
}

AggregateTypeLayoutPtr StructureType::makeLayoutInstance()
{
    auto result = basicMakeObject<AggregateTypeSequentialLayout> ();
    result->setPacked(isPacked_);
    return result;
}

bool StructureType::isPacked() const
{
    return isPacked_;
}

void StructureType::setPacked(bool newPacked)
{
    isPacked_ = newPacked;
}

MethodCategories StructureTypeValue::__typeMethods__()
{
    return MethodCategories{
        {"pattern matching", {
            makeMethodBinding<bool (const TypePtr &)> ("isSequencePatternType", +[](const TypePtr &){
                return true;
            }, MethodFlags::Pure),

            makeMethodBinding<uint64_t (const TypePtr &)> ("sequencePatternTypeMinSize", +[](const TypePtr &structureType) -> uint64_t{
                return structureType.staticAs<StructureType>()->getFieldCount();
            }, MethodFlags::Pure),
            makeMethodBinding<uint64_t (const TypePtr &)> ("sequencePatternTypeMaxSize", +[](const TypePtr &structureType) -> uint64_t {
                return structureType.staticAs<StructureType>()->getFieldCount();
            }, MethodFlags::Pure),
            makeMethodBinding<TypePtr (const TypePtr &, uint64_t)> ("sequencePatternTypeAt:", +[](const TypePtr &structureType, uint64_t i){
                return i < structureType.staticAs<StructureType>()->getFieldCount() ? structureType.staticAs<StructureType>()->getField(i)->getValueType() : nullptr;
            }, MethodFlags::Pure),
        }}
    };
}

MethodCategories StructureTypeValue::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, uint64_t)> ("__sequencePatternAt__:", [=](const MacroInvocationContextPtr &macroContext, uint64_t slotIndex) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                if(slotIndex >= decayedType.staticAs<StructureType>()->getFieldCount())
                    return macroContext->astBuilder->semanticError("Invalid structure slot index.");

                const auto &field = decayedType.staticAs<StructureType>()->getField(slotIndex);
                return macroContext->astBuilder->slotAccess(macroContext->receiverNode, slotIndex, field->getReferenceType());
            }, MethodFlags::Macro),
        }},

        {"pattern matching", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("__sequencePatternSize__", [=](const MacroInvocationContextPtr &macroContext) -> ASTNodePtr {
                auto decayedType = macroContext->selfType->asDecayedType();
                sysmelAssert(decayedType->isStructureType());

                return macroContext->astBuilder->literal(wrapValue(uint64_t(decayedType.staticAs<StructureType> ()->getFieldCount())));
            }, MethodFlags::Macro),
        }},
    };
}

bool StructureTypeValue::isStructureTypeValue() const
{
    return true;
}

AnyValuePtr StructureTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitStructureTypeValue(selfFromThis());
}

SExpression StructureTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(slots.size());
    auto layout = getType().staticAs<StructureType> ()->getLayout().staticAs<AggregateTypeSequentialLayout> ();
    const auto &nonPaddingSlotIndices = layout->getNonPaddingSlotIndices();
    elementsSExpr.elements.reserve(nonPaddingSlotIndices.size());
    for(auto &slotIndex : nonPaddingSlotIndices)
        elementsSExpr.elements.push_back(validAnyValue(slots[slotIndex])->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"struct"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

TypePtr StructureTypeValue::getType() const
{
    return type;
}

} // End of namespace Environment
} // End of namespace Sysmel