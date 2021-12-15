#include "Environment/TypeVisitor.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/ArrayType.hpp"
#include "Environment/FunctionType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/ClosureType.hpp"
#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/MetaType.hpp"
#include "Environment/MethodType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/StructureType.hpp"
#include "Environment/TemporaryReferenceType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/VariantType.hpp"
#include "Environment/BootstrapType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<TypeVisitor> TypeVisitorTypeRegistration;

AnyValuePtr TypeVisitor::visitAnyType(const TypePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr TypeVisitor::visitSimpleType(const SimpleTypePtr &type)
{
    return visitAnyType(type);
}

AnyValuePtr TypeVisitor::visitMetaType(const MetaTypePtr &type)
{
    return visitMetaType(type);
}

AnyValuePtr TypeVisitor::visitBootstrapType(const BootstrapTypePtr &type)
{
    return visitSimpleType(type);
}

AnyValuePtr TypeVisitor::visitDerivedType(const DerivedTypePtr &type)
{
    return visitSimpleType(type);
}

AnyValuePtr TypeVisitor::visitDecoratedType(const DecoratedTypePtr &type)
{
    return visitDerivedType(type);
}

AnyValuePtr TypeVisitor::visitEnumType(const EnumTypePtr &type)
{
    return visitDerivedType(type);
}

AnyValuePtr TypeVisitor::visitPointerLikeType(const PointerLikeTypePtr &type)
{
    return visitDerivedType(type);
}

AnyValuePtr TypeVisitor::visitPointerType(const PointerTypePtr &type)
{
    return visitPointerLikeType(type);
}

AnyValuePtr TypeVisitor::visitReferenceType(const ReferenceTypePtr &type)
{
    return visitPointerLikeType(type);
}

AnyValuePtr TypeVisitor::visitTemporaryReferenceType(const TemporaryReferenceTypePtr &type)
{
    return visitPointerLikeType(type);
}

AnyValuePtr TypeVisitor::visitFunctionalType(const FunctionalTypePtr &type)
{
    return visitSimpleType(type);
}

AnyValuePtr TypeVisitor::visitFunctionType(const FunctionTypePtr &type)
{
    return visitFunctionalType(type);
}

AnyValuePtr TypeVisitor::visitClosureType(const ClosureTypePtr &type)
{
    return visitFunctionalType(type);
}

AnyValuePtr TypeVisitor::visitMethodType(const MethodTypePtr &type)
{
    return visitFunctionalType(type);
}

AnyValuePtr TypeVisitor::visitAggregateType(const AggregateTypePtr &type)
{
    return visitSimpleType(type);
}

AnyValuePtr TypeVisitor::visitArrayType(const ArrayTypePtr &type)
{
    return visitAggregateType(type);
}

AnyValuePtr TypeVisitor::visitTupleType(const TupleTypePtr &type)
{
    return visitAggregateType(type);
}

AnyValuePtr TypeVisitor::visitVariantType(const VariantTypePtr &type)
{
    return visitAggregateType(type);
}

AnyValuePtr TypeVisitor::visitAggregateTypeWithFields(const AggregateTypeWithFieldsPtr &type)
{
    return visitAggregateType(type);
}

AnyValuePtr TypeVisitor::visitStructureType(const StructureTypePtr &type)
{
    return visitAggregateTypeWithFields(type);
}

AnyValuePtr TypeVisitor::visitClassType(const ClassTypePtr &type)
{
    return visitAggregateTypeWithFields(type);
}

AnyValuePtr TypeVisitor::visitUnionType(const UnionTypePtr &type)
{
    return visitAggregateTypeWithFields(type);
}

} // End of namespace Environment
} // End of namespace Sysmel