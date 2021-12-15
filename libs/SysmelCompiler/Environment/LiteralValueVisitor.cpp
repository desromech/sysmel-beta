#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/LiteralCharacter.hpp"
#include "Environment/LiteralInteger.hpp"
#include "Environment/LiteralFraction.hpp"
#include "Environment/LiteralNumber.hpp"
#include "Environment/LiteralFloat.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/StructureType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LiteralValueVisitor> LiteralValueVisitorTypeRegistration;

AnyValuePtr LiteralValueVisitor::visitAnyValue(const AnyValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr LiteralValueVisitor::visitCompilerObject(const CompilerObjectPtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitType(const TypePtr &value)
{
    return visitCompilerObject(value);
}

AnyValuePtr LiteralValueVisitor::visitLiteralValue(const LiteralValuePtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitLiteralNumber(const LiteralNumberPtr &value)
{
    return visitLiteralValue(value);
}

AnyValuePtr LiteralValueVisitor::visitLiteralInteger(const LiteralIntegerPtr &value)
{
    return visitLiteralNumber(value);
}

AnyValuePtr LiteralValueVisitor::visitLiteralCharacter(const LiteralCharacterPtr &value)
{
    return visitLiteralInteger(value);
}

AnyValuePtr LiteralValueVisitor::visitLiteralFraction(const LiteralFractionPtr &value)
{
    return visitLiteralNumber(value);
}

AnyValuePtr LiteralValueVisitor::visitLiteralFloat(const LiteralFloatPtr &value)
{
    return visitLiteralNumber(value);
}

AnyValuePtr LiteralValueVisitor::visitPrimitiveTensorType(const PrimitiveTensorTypePtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitPrimitiveScalarType(const PrimitiveScalarTypePtr &value)
{
    return visitPrimitiveTensorType(value);
}

AnyValuePtr LiteralValueVisitor::visitPrimitiveBooleanType(const PrimitiveBooleanTypePtr &value)
{
    return visitPrimitiveScalarType(value);
}

AnyValuePtr LiteralValueVisitor::visitPrimitiveCharacterType(const PrimitiveCharacterTypePtr &value)
{
    return visitPrimitiveIntegerType(value);
}

AnyValuePtr LiteralValueVisitor::visitPrimitiveIntegerType(const PrimitiveIntegerTypePtr &value)
{
    return visitPrimitiveScalarType(value);
}

AnyValuePtr LiteralValueVisitor::visitPrimitiveFloatType(const PrimitiveFloatTypePtr &value)
{
    return visitPrimitiveScalarType(value);
}

AnyValuePtr LiteralValueVisitor::visitDecoratedTypeValue(const DecoratedTypeValuePtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitEnumTypeValue(const EnumTypeValuePtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitPointerLikeTypeValue(const PointerLikeTypeValuePtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitPointerTypeValue(const PointerTypeValuePtr &value)
{
    return visitPointerLikeTypeValue(value);
}

AnyValuePtr LiteralValueVisitor::visitReferenceTypeValue(const ReferenceTypeValuePtr &value)
{
    return visitPointerLikeTypeValue(value);
}

AnyValuePtr LiteralValueVisitor::visitTemporaryReferenceTypeValue(const TemporaryReferenceTypeValuePtr &value)
{
    return visitPointerLikeTypeValue(value);
}

AnyValuePtr LiteralValueVisitor::visitAggregateTypeValue(const AggregateTypeValuePtr &value)
{
    return visitAnyValue(value);
}

AnyValuePtr LiteralValueVisitor::visitAggregateTypeWithFieldsValue(const AggregateTypeWithFieldsValuePtr &value)
{
    return visitAggregateTypeValue(value);
}

AnyValuePtr LiteralValueVisitor::visitStructureTypeValue(const StructureTypeValuePtr &value)
{
    return visitAggregateTypeWithFieldsValue(value);
}

AnyValuePtr LiteralValueVisitor::visitClassTypeValue(const ClassTypeValuePtr &value)
{
    return visitAggregateTypeWithFieldsValue(value);
}

AnyValuePtr LiteralValueVisitor::visitUnionTypeValue(const UnionTypeValuePtr &value)
{
    return visitAggregateTypeWithFieldsValue(value);
}

} // End of namespace Environment
} // End of namespace Sysmel