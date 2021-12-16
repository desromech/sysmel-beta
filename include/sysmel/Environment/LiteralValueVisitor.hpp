#ifndef SYSMEL_ENVIRONMENT_LITERAL_VALUE_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_VALUE_VISITOR_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Undefined);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Void);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralValueVisitor);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralBoolean);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralCharacter);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralNumber);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralFloat);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralFraction);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralInteger);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralString);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralSymbol);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveTensorType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveScalarType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveBooleanType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveCharacterType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveIntegerType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveFloatType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(DecoratedTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerLikeTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceTypeValue);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeWithFieldsValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(StructureTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassTypeValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionTypeValue);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class LiteralValueVisitor : public SubtypeOf<CompilerObject, LiteralValueVisitor>
{
public:
    static constexpr char const __typeName__[] = "LiteralValueVisitor";

    virtual AnyValuePtr visitAnyValue(const AnyValuePtr &value);
    virtual AnyValuePtr visitCompilerObject(const CompilerObjectPtr &value);
    virtual AnyValuePtr visitType(const TypePtr &value);
    virtual AnyValuePtr visitUndefined(const UndefinedPtr &value);
    virtual AnyValuePtr visitVoid(const VoidPtr &value);

    virtual AnyValuePtr visitLiteralValue(const LiteralValuePtr &value);
    virtual AnyValuePtr visitLiteralBoolean(const LiteralBooleanPtr &value);
    virtual AnyValuePtr visitLiteralCharacter(const LiteralCharacterPtr &value);
    virtual AnyValuePtr visitLiteralNumber(const LiteralNumberPtr &value);
    virtual AnyValuePtr visitLiteralInteger(const LiteralIntegerPtr &value);
    virtual AnyValuePtr visitLiteralFraction(const LiteralFractionPtr &value);
    virtual AnyValuePtr visitLiteralFloat(const LiteralFloatPtr &value);
    virtual AnyValuePtr visitLiteralString(const LiteralStringPtr &value);
    virtual AnyValuePtr visitLiteralSymbol(const LiteralSymbolPtr &value);

    virtual AnyValuePtr visitPrimitiveTensorType(const PrimitiveTensorTypePtr &value);
    virtual AnyValuePtr visitPrimitiveScalarType(const PrimitiveScalarTypePtr &value);
    virtual AnyValuePtr visitPrimitiveBooleanType(const PrimitiveBooleanTypePtr &value);
    virtual AnyValuePtr visitPrimitiveCharacterType(const PrimitiveCharacterTypePtr &value);
    virtual AnyValuePtr visitPrimitiveIntegerType(const PrimitiveIntegerTypePtr &value);
    virtual AnyValuePtr visitPrimitiveFloatType(const PrimitiveFloatTypePtr &value);

    virtual AnyValuePtr visitDecoratedTypeValue(const DecoratedTypeValuePtr &value);
    virtual AnyValuePtr visitEnumTypeValue(const EnumTypeValuePtr &value);
    virtual AnyValuePtr visitPointerLikeTypeValue(const PointerLikeTypeValuePtr &value);
    virtual AnyValuePtr visitPointerTypeValue(const PointerTypeValuePtr &value);
    virtual AnyValuePtr visitReferenceTypeValue(const ReferenceTypeValuePtr &value);
    virtual AnyValuePtr visitTemporaryReferenceTypeValue(const TemporaryReferenceTypeValuePtr &value);
    virtual AnyValuePtr visitAggregateTypeValue(const AggregateTypeValuePtr &value);
    virtual AnyValuePtr visitArrayTypeValue(const ArrayTypeValuePtr &value);
    virtual AnyValuePtr visitTupleTypeValue(const TupleTypeValuePtr &value);
    virtual AnyValuePtr visitVariantTypeValue(const VariantTypeValuePtr &value);
    virtual AnyValuePtr visitAggregateTypeWithFieldsValue(const AggregateTypeWithFieldsValuePtr &value);
    virtual AnyValuePtr visitStructureTypeValue(const StructureTypeValuePtr &value);
    virtual AnyValuePtr visitClassTypeValue(const ClassTypeValuePtr &value);
    virtual AnyValuePtr visitUnionTypeValue(const UnionTypeValuePtr &value);
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_VALUE_VISITOR_HPP