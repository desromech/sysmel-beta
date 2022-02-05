#include "Environment/ReferenceType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/IdentityTypeConversionRule.hpp"
#include "Environment/ValueAsVoidTypeConversionRule.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ReferenceTypeValue> ReferenceTypeValueTypeRegistration;

ReferenceTypePtr ReferenceType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, nullptr);
}

ReferenceTypePtr ReferenceType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto canonicalAddressSpace = addressSpace;
    if(validAnyValue(addressSpace)->isUndefined())
        canonicalAddressSpace = internSymbol("generic");
    auto &cache = RuntimeContext::getActive()->referenceTypeCache;
    auto it = cache.find({baseType, canonicalAddressSpace});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<ReferenceType> ();
    result->setSupertypeAndImplicitMetaType(ReferenceTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = canonicalAddressSpace;
    cache.insert({{baseType, canonicalAddressSpace}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool ReferenceType::isReferenceType() const
{
    return true;
}

bool ReferenceType::isReferenceLikeType() const
{
    return true;
}

AnyValuePtr ReferenceType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitReferenceType(selfFromThis());
}

AnyValuePtr ReferenceType::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    return baseType->lookupLocalSymbolFromScope(symbol, accessingScope);
}

AnyValuePtr ReferenceType::lookupLocalSymbolRecursivelyFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    return baseType->lookupLocalSymbolRecursivelyFromScope(symbol, accessingScope);
}

ReferenceTypePtr ReferenceType::ref()
{
    return selfFromThis();
}

ReferenceTypePtr ReferenceType::refFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return selfFromThis();

    return baseType->refFor(newAddressSpace);
}

PointerLikeTypePtr ReferenceType::tempRef()
{
    return selfFromThis();
}

PointerLikeTypePtr ReferenceType::tempRefFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return selfFromThis();

    return baseType->refFor(newAddressSpace);
}

TypePtr ReferenceType::withDecorations(TypeDecorationFlags decorations)
{
    (void)decorations;
    return selfFromThis();
}

TypePtr ReferenceType::asDecayedType()
{
    return baseType->asDecayedType();
}

bool ReferenceType::isConstOrConstReferenceType() const
{
    return baseType->isConstDecoratedType();
}

TypePtr ReferenceType::asConstOrConstReferenceType()
{
    if(baseType->isConstDecoratedType())
        return selfFromThis();
    return baseType->withConst()->refFor(addressSpace);
}

TypePtr ReferenceType::asSuperReceiverType()
{
    auto superType = baseType->asUndecoratedType()->getSupertype();
    if(!superType)
        return nullptr;

    return superType->withDecorations(baseType->getDecorationFlags())->refFor(addressSpace);
}

std::string ReferenceType::getQualifiedName() const
{
    if(hasGenericAddressSpace())
        return baseType->getQualifiedName() + " ref";
    return "(" + baseType->getQualifiedName() + " refFor: " + addressSpace->printString();
}

SExpression ReferenceType::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"refType"}},
        baseType->asSExpression(),
        addressSpace->asSExpression()
    }};
}

PointerLikeTypeValuePtr ReferenceType::makeWithValue(const AnyValuePtr &value)
{
    auto reference = basicMakeObject<ReferenceTypeValue> ();
    reference->type = selfFromThis();
    reference->baseValue = value;
    return reference;
}

void ReferenceType::addSpecializedInstanceMethods()
{
    auto pointerType = baseType->pointerFor(addressSpace);
    addMethodCategories(MethodCategories{
            {"accessing", {
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr)> ("reference.to.pointer", "address", selfFromThis(), pointerType, {}, [=](const ReferenceTypeValuePtr &self) {
                    return pointerType->makeWithValue(self->baseValue);
                }, MethodFlags::Pure),
            }
        }
    });

    if(baseType->isConstDecoratedType())
    {
        auto nonConstBaseType = baseType->asUndecoratedType();
        auto nonConstRefType = nonConstBaseType->refFor(addressSpace);
        addConstructor(makeIntrinsicConstructorWithSignature<AnyValuePtr (ReferenceTypePtr, ReferenceTypeValuePtr)> ("reference.reinterpret", getType(), selfFromThis(), {nonConstRefType}, [=](const ReferenceTypePtr &self, const ReferenceTypeValuePtr &nonConstRef) {
            return self->makeWithValue(nonConstRef->baseValue);
        }, MethodFlags::Pure));
    }

    addConversion(makeIntrinsicConversionWithSignature<AnyValuePtr (ReferenceTypeValuePtr)> (baseType->asUndecoratedType()->isReturnedByReference() ? "reference.load.aggregate" : "reference.load.value", selfFromThis(), baseType, {}, [=](const ReferenceTypeValuePtr &self) {
        return validAnyValue(self->baseValue)->accessVariableAsValueWithType(baseType);
    }));

    // Define the assignment
    if(!baseType->isConstDecoratedType() && !baseType->isPassedByReference() &&
        baseType->hasTrivialAssignCopyingFrom() && baseType->hasTrivialAssignMovingFrom())
    {
        addMethodCategories(MethodCategories{
                {"assignment", {
                    makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr, AnyValuePtr)> ("reference.copy.assignment.trivial", ":=", selfFromThis(), selfFromThis(), {baseType}, [=](const ReferenceTypeValuePtr &self, const AnyValuePtr &newValue) {
                        self->baseValue->copyAssignValue(newValue);
                        return self;
                    }),
                }
            }
        });
    }
}

TypePtr ReferenceType::asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    switch(mode)
    {
    case TypeInferenceMode::Value:
    default:
        return baseType->asInferredTypeForWithModeInEnvironment(node, mode, isMutable, concreteLiterals, environment);
    case TypeInferenceMode::Reference:
    case TypeInferenceMode::TemporaryReference:
        return selfFromThis();
    }
}

ASTNodePtr ReferenceType::analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return baseType->analyzeMessageSendNode(partiallyAnalyzedNode, semanticAnalyzer);
}

bool ReferenceTypeValue::isReferenceTypeValue() const
{
    return true;
}

bool ReferenceTypeValue::isReferenceLikeTypeValue() const
{
    return true;
}

TypePtr ReferenceTypeValue::getType() const
{
    return type;
}

SExpression ReferenceTypeValue::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"refValue"}},
        type->asSExpression(),
        baseValue ? baseValue->asSExpression() : nullptr
    }};
}

AnyValuePtr ReferenceTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitReferenceTypeValue(selfFromThis());
}

AnyValuePtr ReferenceTypeValue::getReferenceToFieldWithType(const FieldVariablePtr &field, const TypePtr &referenceType)
{
    return validAnyValue(baseValue)->getReferenceToFieldWithType(field, referenceType);
}

AnyValuePtr ReferenceTypeValue::getReferenceToSlotWithType(int64_t slotIndex, int64_t slotOffset, const TypePtr &referenceType)
{
    return validAnyValue(baseValue)->getReferenceToSlotWithType(slotIndex, slotOffset, referenceType);
}

} // End of namespace Environment
} // End of namespace Sysmel