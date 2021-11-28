#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_TYPE_HPP
#pragma once

#include "DerivedType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumTypeValue);

/**
 * I am an instance of a function type object.
 */
class EnumType : public SubMetaTypeOf<DerivedType, EnumType>
{
public:
    virtual bool isEnumType() const override;

    virtual const TypePtr &getBaseType() override;

    virtual bool supportsDynamicCompileTimeMessageSend() const override;

    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialCopyingFrom() override;
    virtual bool hasTrivialMovingFrom() override;

    virtual void enqueuePendingValueTypeCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);
    virtual void enqueuePendingValuesCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);

    void addValue(const ASTNodePtr &position, const AnyValuePtr &key, const AnyValuePtr &value);
    AnyValuePtr lookupRawValue(const AnyValuePtr &symbol);

    void addValues(const ASTNodePtr &position, const AnyValuePtr &newValues);

protected:
    void addSpecializedMethods();

    virtual void evaluateAllPendingCodeFragments() override;
    virtual void evaluateAllPendingBodyBlockCodeFragments() override;

    void evaluatePendingValueTypeCodeFragments();
    void evaluatePendingValuesFragments();

    bool hasEvaluatedValueType = false;
    DeferredCompileTimeCodeFragmentPtrList pendingValueTypeCodeFragments;
    DeferredCompileTimeCodeFragmentPtrList pendingValuesCodeFragments;

    std::unordered_map<AnyValuePtr, AnyValuePtr> rawValues;
    std::unordered_map<AnyValuePtr, AnyValuePtr> values;
};

/**
 * I am an instance of a function type object.
 */
class EnumTypeValue : public SubtypeOf<DerivedTypeValue, EnumTypeValue>
{
public:
    static constexpr char const __typeName__[] = "EnumType";
    static constexpr char const __sysmelTypeName__[] = "_EnumType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isEnumTypeValue() const override;
    virtual TypePtr getType() const override;

    virtual bool unwrapAsBoolean() const override;
    virtual uint8_t unwrapAsUInt8() const override;
    virtual int8_t unwrapAsInt8() const override;
    virtual uint16_t unwrapAsUInt16() const override;
    virtual int16_t unwrapAsInt16() const override;
    virtual uint32_t unwrapAsUInt32() const override;
    virtual int32_t unwrapAsInt32() const override;
    virtual uint64_t unwrapAsUInt64() const override;
    virtual int64_t unwrapAsInt64() const override;
    virtual LargeInteger unwrapAsLargeInteger() const override;
    virtual Fraction unwrapAsFraction() const override;
    virtual char unwrapAsChar8() const override;
    virtual char16_t unwrapAsChar16() const override;
    virtual char32_t unwrapAsChar32() const override;
    virtual float unwrapAsFloat32() const override;
    virtual double unwrapAsFloat64() const override;
    virtual std::string unwrapAsString() const override;
    virtual AnyValuePtrList unwrapAsArray() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_TYPE_HPP