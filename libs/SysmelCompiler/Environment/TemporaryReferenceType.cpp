#include "Environment/TemporaryReferenceType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/IdentityTypeConversionRule.hpp"
#include "Environment/ValueAsVoidTypeConversionRule.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<TemporaryReferenceTypeValue> TemporaryReferenceTypeValueTypeRegistration;

TemporaryReferenceTypePtr TemporaryReferenceType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, internSymbol("generic"));
}

TemporaryReferenceTypePtr TemporaryReferenceType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto &cache = RuntimeContext::getActive()->temporaryReferenceTypeCache;
    auto it = cache.find({baseType, addressSpace});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<TemporaryReferenceType> ();
    result->setSupertypeAndImplicitMetaType(TemporaryReferenceTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = addressSpace;
    cache.insert({{baseType, addressSpace}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool TemporaryReferenceType::isReferenceLikeType() const
{
    return true;
}

bool TemporaryReferenceType::isTemporaryReferenceType() const
{
    return true;
}

AnyValuePtr TemporaryReferenceType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitTemporaryReferenceType(selfFromThis());
}

AnyValuePtr TemporaryReferenceType::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    return baseType->lookupLocalSymbolFromScope(symbol, accessingScope);
}

ReferenceTypePtr TemporaryReferenceType::ref()
{
    return baseType->ref();
}

ReferenceTypePtr TemporaryReferenceType::refFor(const AnyValuePtr &newAddressSpace)
{
    return baseType->refFor(newAddressSpace);
}

PointerLikeTypePtr TemporaryReferenceType::tempRef()
{
    return selfFromThis();
}

PointerLikeTypePtr TemporaryReferenceType::tempRefFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return selfFromThis();

    return baseType->tempRefFor(newAddressSpace);
}

TypePtr TemporaryReferenceType::withDecorations(TypeDecorationFlags decorations)
{
    (void)decorations;
    return selfFromThis();
}

std::string TemporaryReferenceType::printString() const
{
    if(hasGenericAddressSpace())
        return baseType->printString() + " tempRef";
    return "(" + baseType->printString() + " tempRefFor: " + addressSpace->printString();
}

SExpression TemporaryReferenceType::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"tempRefType"}},
        baseType->asSExpression(),
        addressSpace->asSExpression()
    }};
}

PointerLikeTypeValuePtr TemporaryReferenceType::makeWithValue(const AnyValuePtr &value)
{
    auto temporaryReference = basicMakeObject<TemporaryReferenceTypeValue> ();
    temporaryReference->type = selfFromThis();
    temporaryReference->baseValue = value;
    return temporaryReference;
}

void TemporaryReferenceType::addSpecializedInstanceMethods()
{
    auto pointerType = baseType->pointerFor(addressSpace);

    addMethodCategories(MethodCategories{
            {"accessing", {
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (TemporaryReferenceTypeValuePtr)> ("reference.to.pointer", "address", selfFromThis(), pointerType, {}, [=](const TemporaryReferenceTypeValuePtr &value) {
                    return pointerType->makeWithValue(value->baseValue);
                }, MethodFlags::Pure),
            }
        }
    });
}

TypePtr TemporaryReferenceType::asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    switch(mode)
    {
    case TypeInferenceMode::Value:
    default:
        return baseType->asInferredTypeForWithModeInEnvironment(node, mode, isMutable, concreteLiterals, environment);
    case TypeInferenceMode::Reference:
        return ref()->asInferredTypeForWithModeInEnvironment(node, mode, isMutable, concreteLiterals, environment);
    case TypeInferenceMode::TemporaryReference:
        return selfFromThis();
    }
}

ASTNodePtr TemporaryReferenceType::analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return baseType->analyzeMessageSendNode(partiallyAnalyzedNode, semanticAnalyzer);
}

bool TemporaryReferenceTypeValue::isTemporaryReferenceTypeValue() const
{
    return true;
}

bool TemporaryReferenceTypeValue::isReferenceLikeTypeValue() const
{
    return true;
}

TypePtr TemporaryReferenceTypeValue::getType() const
{
    return type;
}

SExpression TemporaryReferenceTypeValue::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"tempRefValue"}},
        type->asSExpression(),
        baseValue ? baseValue->asSExpression() : nullptr
    }};
}

AnyValuePtr TemporaryReferenceTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitTemporaryReferenceTypeValue(selfFromThis());
}

AnyValuePtr TemporaryReferenceTypeValue::getReferenceToFieldWithType(const FieldVariablePtr &field, const TypePtr &referenceType)
{
    return validAnyValue(baseValue)->getReferenceToFieldWithType(field, referenceType);
}

AnyValuePtr TemporaryReferenceTypeValue::getReferenceToSlotWithType(int64_t slotIndex, int64_t slotOffset, const TypePtr &referenceType)
{
    return validAnyValue(baseValue)->getReferenceToSlotWithType(slotIndex, slotOffset, referenceType);
}

} // End of namespace Environment
} // End of namespace Sysmel
