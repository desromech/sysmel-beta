#include "Environment/EnumType.hpp"
#include "Environment/EnumConstant.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/Wrappers.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/LiteralValueVisitor.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<EnumTypeValue> EnumTypeValueTypeRegistration;

bool EnumType::isEnumType() const
{
    return true;
}

AnyValuePtr EnumType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitEnumType(selfFromThis());
}

const TypePtr &EnumType::getBaseType()
{
    evaluatePendingValueTypeCodeFragments();
    return SuperType::getBaseType();
}

bool EnumType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool EnumType::isNullableType() const
{
    return false;
}

bool EnumType::isImmutableType()
{
    return getBaseType()->isImmutableType();
}

bool EnumType::hasTrivialInitialization()
{
    return getBaseType()->hasTrivialInitialization();
}

bool EnumType::hasTrivialInitializationCopyingFrom()
{
    return getBaseType()->hasTrivialInitializationCopyingFrom();
}

bool EnumType::hasTrivialInitializationMovingFrom()
{
    return getBaseType()->hasTrivialInitializationMovingFrom();
}

bool EnumType::hasTrivialFinalization()
{
    return getBaseType()->hasTrivialFinalization();
}

bool EnumType::hasTrivialAssignCopyingFrom()
{
    return getBaseType()->hasTrivialAssignCopyingFrom();
}

bool EnumType::hasTrivialAssignMovingFrom()
{
    return getBaseType()->hasTrivialAssignMovingFrom();
}

uint64_t EnumType::getMemorySize()
{
    return getBaseType()->getMemorySize();
}

uint64_t EnumType::getMemoryAlignment()
{
    return getBaseType()->getMemorySize();
}

void EnumType::addSpecializedMethods()
{
    sysmelAssert(baseType);
    addConstructor(makeIntrinsicConstructorWithSignature<EnumTypeValuePtr (TypePtr, AnyValuePtr)> ("enum.wrap", getType(), selfFromThis(), {baseType}, +[](const TypePtr &selfType, const AnyValuePtr &value) {
        auto wrappedValue = basicMakeObject<EnumTypeValue> ();
        wrappedValue->type = staticObjectCast<EnumType> (selfType);
        wrappedValue->baseValue = value;
        return wrappedValue;
    }, MethodFlags::Pure | MethodFlags::Explicit));

    addMethodCategories(MethodCategories{
            {"accessing", {
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (EnumTypeValuePtr)> ("enum.unwrap", "value", selfFromThis(), baseType, {}, +[](const EnumTypeValuePtr &value) {
                    return value->baseValue;
                }, MethodFlags::Pure | MethodFlags::Conversion | MethodFlags::Explicit),
            }
        }
    });
}

void EnumType::enqueuePendingValueTypeCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    pendingValueTypeCodeFragments.push_back(codeFragment);
    enqueuePendingSemanticAnalysis();
}

void EnumType::enqueuePendingValuesCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    pendingValuesCodeFragments.push_back(codeFragment);
    enqueuePendingSemanticAnalysis();
}

void EnumType::evaluatePendingValueTypeCodeFragments()
{
    while(!pendingValueTypeCodeFragments.empty())
    {
        auto toEvaluate = pendingValueTypeCodeFragments;
        pendingValueTypeCodeFragments.clear();
        for(auto &fragment : toEvaluate)
        {
            auto newBaseType = fragment->analyzeAndEvaluateAsTypeExpression();
            if(!hasEvaluatedValueType)
            {
                baseType = newBaseType;
                hasEvaluatedValueType = true;
                addSpecializedMethods();
            }
            else
            {
                // Raise an error.
                if(baseType != newBaseType)
                {
                    auto errorNode = basicMakeObject<ASTSemanticErrorNode> ();
                    errorNode->sourcePosition = fragment->codeFragment->sourcePosition;
                    errorNode->errorMessage = formatString("Conflicting value type ({2} vs {3}) given for {1}.", {{
                        baseType->printString(), newBaseType->printString(), printString()
                    }});
                    errorNode->asCompilationError()->signal();
                }
            }
        }
    }

    hasEvaluatedValueType = true;
}

void EnumType::evaluatePendingValuesFragments()
{
    evaluatePendingValueTypeCodeFragments();
    while(!pendingValuesCodeFragments.empty())
    {
        auto toEvaluate = pendingValuesCodeFragments;
        pendingValuesCodeFragments.clear();
        for(auto &fragment : toEvaluate)
            fragment->analyzeAndEvaluateAsValuesForEnumType(selfFromThis());
    }
}

void EnumType::evaluateAllPendingCodeFragments()
{
    evaluatePendingValueTypeCodeFragments();
    evaluatePendingValuesFragments();
    evaluateAllPendingBodyBlockCodeFragments();
}

void EnumType::evaluateAllPendingBodyBlockCodeFragments()
{
    evaluatePendingValuesFragments();
    SuperType::evaluateAllPendingBodyBlockCodeFragments();
}

void EnumType::addValue(const ASTNodePtr &position, const AnyValuePtr &key, const AnyValuePtr &value)
{
    rawValues[key] = validAnyValue(value);

    auto wrappedValue = basicMakeObject<EnumTypeValue> ();
    wrappedValue->type = selfFromThis();
    wrappedValue->baseValue = value;
    values[key] = wrappedValue;

    auto constant = basicMakeObject<EnumConstant> ();
    constant->setName(key);
    constant->setValue(wrappedValue);
    constant->setDefinitionNode(position);
    recordChildProgramEntityDefinition(constant);
    bindProgramEntityWithVisibility(constant, ProgramEntityVisibility::Public);
}

AnyValuePtr EnumType::lookupRawValue(const AnyValuePtr &symbol)
{
    auto it = rawValues.find(symbol);
    return it != rawValues.end() ? it->second : nullptr;
}

void EnumType::addValues(const ASTNodePtr &position, const AnyValuePtr &newValues)
{
    auto associations = newValues->unwrapAsArray();
    for(auto &assoc : associations)
    {
        auto key = assoc->perform<AnyValuePtr> ("key");
        auto value = assoc->perform<AnyValuePtr> ("value");
        addValue(position, key, value);
    }
}

bool EnumTypeValue::isEnumTypeValue() const
{
    return true;
}

AnyValuePtr EnumTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitEnumTypeValue(selfFromThis());
}

TypePtr EnumTypeValue::getType() const
{
    return type;
}

bool EnumTypeValue::unwrapAsBoolean() const
{
    return validAnyValue(baseValue)->unwrapAsBoolean();
}

uint8_t EnumTypeValue::unwrapAsUInt8() const
{
    return validAnyValue(baseValue)->unwrapAsUInt8();
}

int8_t EnumTypeValue::unwrapAsInt8() const
{
    return validAnyValue(baseValue)->unwrapAsInt8();
}

uint16_t EnumTypeValue::unwrapAsUInt16() const
{
    return validAnyValue(baseValue)->unwrapAsUInt16();
}

int16_t EnumTypeValue::unwrapAsInt16() const
{
    return validAnyValue(baseValue)->unwrapAsInt16();
}

uint32_t EnumTypeValue::unwrapAsUInt32() const
{
    return validAnyValue(baseValue)->unwrapAsUInt32();
}

int32_t EnumTypeValue::unwrapAsInt32() const
{
    return validAnyValue(baseValue)->unwrapAsInt32();
}

uint64_t EnumTypeValue::unwrapAsUInt64() const
{
    return validAnyValue(baseValue)->unwrapAsUInt64();
}

int64_t EnumTypeValue::unwrapAsInt64() const
{
    return validAnyValue(baseValue)->unwrapAsInt64();
}

LargeInteger EnumTypeValue::unwrapAsLargeInteger() const
{
    return validAnyValue(baseValue)->unwrapAsLargeInteger();
}

Fraction EnumTypeValue::unwrapAsFraction() const
{
    return validAnyValue(baseValue)->unwrapAsFraction();
}

char EnumTypeValue::unwrapAsChar8() const
{
    return validAnyValue(baseValue)->unwrapAsChar8();
}

char16_t EnumTypeValue::unwrapAsChar16() const
{
    return validAnyValue(baseValue)->unwrapAsChar16();
}

char32_t EnumTypeValue::unwrapAsChar32() const
{
    return validAnyValue(baseValue)->unwrapAsChar32();
}

float EnumTypeValue::unwrapAsFloat32() const
{
    return validAnyValue(baseValue)->unwrapAsFloat32();
}

double EnumTypeValue::unwrapAsFloat64() const
{
    return validAnyValue(baseValue)->unwrapAsFloat64();
}

std::string EnumTypeValue::unwrapAsString() const
{
    return validAnyValue(baseValue)->unwrapAsString();
}

AnyValuePtrList EnumTypeValue::unwrapAsArray() const
{
    return validAnyValue(baseValue)->unwrapAsArray();
}

} // End of namespace Environment
} // End of namespace Sysmel