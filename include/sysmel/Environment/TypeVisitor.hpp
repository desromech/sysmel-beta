#ifndef SYSMEL_ENVIRONMENT_TYPE_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_TYPE_VISITOR_HPP
#pragma once

#include "Type.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SimpleType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MetaType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(DerivedType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DecoratedType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerLikeType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeWithFields);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(StructureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveVectorType);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PaddingType);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT TypeVisitor : public SubtypeOf<CompilerObject, TypeVisitor>
{
public:
    static constexpr char const __typeName__[] = "TypeVisitor";

    virtual AnyValuePtr visitAnyType(const TypePtr &type);
    virtual AnyValuePtr visitSimpleType(const SimpleTypePtr &type);
    virtual AnyValuePtr visitMetaType(const MetaTypePtr &type);
    virtual AnyValuePtr visitBootstrapType(const BootstrapTypePtr &type);

    virtual AnyValuePtr visitDerivedType(const DerivedTypePtr &type);
    virtual AnyValuePtr visitDecoratedType(const DecoratedTypePtr &type);
    virtual AnyValuePtr visitEnumType(const EnumTypePtr &type);

    virtual AnyValuePtr visitPointerLikeType(const PointerLikeTypePtr &type);
    virtual AnyValuePtr visitPointerType(const PointerTypePtr &type);
    virtual AnyValuePtr visitReferenceType(const ReferenceTypePtr &type);
    virtual AnyValuePtr visitTemporaryReferenceType(const TemporaryReferenceTypePtr &type);

    virtual AnyValuePtr visitFunctionalType(const FunctionalTypePtr &type);
    virtual AnyValuePtr visitFunctionType(const FunctionTypePtr &type);
    virtual AnyValuePtr visitClosureType(const ClosureTypePtr &type);
    virtual AnyValuePtr visitMethodType(const MethodTypePtr &type);

    virtual AnyValuePtr visitAggregateType(const AggregateTypePtr &type);
    virtual AnyValuePtr visitArrayType(const ArrayTypePtr &type);
    virtual AnyValuePtr visitTupleType(const TupleTypePtr &type);
    virtual AnyValuePtr visitVariantType(const VariantTypePtr &type);

    virtual AnyValuePtr visitAggregateTypeWithFields(const AggregateTypeWithFieldsPtr &type);
    virtual AnyValuePtr visitStructureType(const StructureTypePtr &type);
    virtual AnyValuePtr visitClassType(const ClassTypePtr &type);
    virtual AnyValuePtr visitUnionType(const UnionTypePtr &type);

    virtual AnyValuePtr visitPrimitiveVectorType(const PrimitiveVectorTypePtr &type);
    virtual AnyValuePtr visitPaddingType(const PaddingTypePtr &type);
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_VALUE_VISITOR_HPP