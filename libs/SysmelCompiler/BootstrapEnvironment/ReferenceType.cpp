#include "sysmel/BootstrapEnvironment/ReferenceType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/PointerType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ReferenceTypeValue> ReferenceTypeValueTypeRegistration;

ReferenceTypePtr ReferenceType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, internSymbol("generic"));
}

ReferenceTypePtr ReferenceType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto &cache = RuntimeContext::getActive()->referenceTypeCache;
    auto it = cache.find({baseType, addressSpace});
    if(it != cache.end())
        return it->second;

    auto result = std::make_shared<ReferenceType> ();
    result->setSupertypeAndImplicitMetaType(ReferenceTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = addressSpace;
    cache.insert({{baseType, addressSpace}, result});
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

ReferenceTypePtr ReferenceType::ref()
{
    return shared_from_this();
}

ReferenceTypePtr ReferenceType::refFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return shared_from_this();

    return baseType->refFor(newAddressSpace);
}

PointerLikeTypePtr ReferenceType::tempRef()
{
    return shared_from_this();
}

PointerLikeTypePtr ReferenceType::tempRefFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return shared_from_this();

    return baseType->refFor(newAddressSpace);
}

std::string ReferenceType::printString() const
{
    if(hasGenericAddressSpace())
        return baseType->printString() + " ref";
    return "(" + baseType->printString() + " refFor: " + addressSpace->printString();
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
    auto reference = std::make_shared<ReferenceTypeValue> ();
    reference->type = shared_from_this();
    reference->baseValue = value;
    return reference;
}

void ReferenceType::addSpecializedInstanceMethods()
{
    auto pointerType = baseType->pointerFor(addressSpace);

    addMethodCategories(MethodCategories{
            {"accessing", {
                makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr)> ("reference.to.pointer", "address", shared_from_this(), pointerType, {}, [=](const ReferenceTypeValuePtr &self) {
                    return pointerType->makeWithValue(self->baseValue);
                }, MethodFlags::Pure),
            }
        }
    });

    addConversion(makeIntrinsicConversionWithSignature<AnyValuePtr (ReferenceTypeValuePtr)> ("reference.load", shared_from_this(), baseType, {}, [=](const ReferenceTypeValuePtr &self) {
        return validAnyValue(self->baseValue)->accessVariableAsValueWithType(baseType);
    }));

    // Define the assignment
    // TODO: Depend on the triviality of assignment.
    if(baseType->isImmutableType())
    {
        addMethodCategories(MethodCategories{
                {"assignment", {
                    makeIntrinsicMethodBindingWithSignature<PointerLikeTypeValuePtr (ReferenceTypeValuePtr, AnyValuePtr)> ("reference.copy.assignment.trivial", ":=", shared_from_this(), shared_from_this(), {baseType}, [=](const ReferenceTypeValuePtr &self, const AnyValuePtr &newValue) {
                        self->baseValue->copyAssignValue(newValue);
                        return self;
                    }),
                }
            }
        });
    }
}

TypePtr ReferenceType::asInferredTypeWithMode(TypeInferenceMode mode, bool isMutable)
{
    switch(mode)
    {
    case TypeInferenceMode::Value:
    default:
        return baseType->asInferredTypeWithMode(mode, isMutable);
    case TypeInferenceMode::Reference:
    case TypeInferenceMode::TemporaryReference:
        return shared_from_this();
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius