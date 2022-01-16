#include "Backend/LLVM/LLVMDebugTypeVisitor.hpp"

#include "Environment/ASTSourcePosition.hpp"

#include "Environment/FieldVariable.hpp"
#include "Environment/NameMangler.hpp"

#include "Environment/MetaType.hpp"

#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"

#include "Environment/ArrayType.hpp"
#include "Environment/PrimitiveVectorType.hpp"

#include "Environment/AggregateTypeSequentialLayout.hpp"

#include "Environment/StructureType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/TupleType.hpp"

#include "Environment/SSAProgramEntity.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace LLVM
{

/**
 * I am the base interface for a SSA based code generation backend
 */
class LLVMDebugType : public SubtypeOf<CompilerObject, LLVMDebugType>
{
public:
    static constexpr char const __typeName__[] = "LLVMDebugType";

    llvm::DIType *type = nullptr;
};

static BootstrapTypeRegistration<LLVMDebugTypeVisitor> LLVMDebugTypeVisitorTypeRegistration;
static BootstrapTypeRegistration<LLVMDebugType> LLVMDebugTypeTypeRegistration;

static AnyValuePtr wrapLLVMDebugType(llvm::DIType *type)
{
    auto wrapper = basicMakeObject<LLVMDebugType> ();
    wrapper->type = type;
    return wrapper;
}

llvm::DIType *LLVMDebugTypeVisitor::visitType(const TypePtr &type)
{
    return type->acceptTypeVisitor(selfFromThis()).staticAs<LLVMDebugType> ()->type;
}

AnyValuePtr LLVMDebugTypeVisitor::visitMetaType(const MetaTypePtr &type)
{    
    llvm::DIScope* scope = nullptr;

    auto builder = backend->getDIBuilder();

    auto parentProgramEntity = type->getParentProgramEntity();
    if(parentProgramEntity)
        scope = backend->getOrCreateDIScopeForSSAProgramEntity(staticObjectCast<SSAProgramEntity> (parentProgramEntity->asSSAValueRequiredInPosition(ASTSourcePosition::empty())));

    auto name = type->getValidNameStringIncludingTemplateName();
    auto line = 0;
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;
    auto metaType = builder->createStructType(
        scope, name, nullptr, line,
        size, uint32_t(alignment),
        llvm::DINode::FlagTypePassByReference, nullptr, 
        builder->getOrCreateArray({}),
        0, nullptr,
        backend->getNameMangler()->mangleTypeInfo(type)
    );

    return wrapLLVMDebugType(metaType);
}

AnyValuePtr LLVMDebugTypeVisitor::visitDecoratedType(const DecoratedTypePtr &type)
{
    auto baseType = backend->translateDIType(type->getBaseType());
    auto result = baseType;
    if(type->isRestrictDecoratedType())
        result = backend->getDIBuilder()->createQualifiedType(llvm::dwarf::DW_TAG_restrict_type, result);
    if(type->isVolatileDecoratedType())
        result = backend->getDIBuilder()->createQualifiedType(llvm::dwarf::DW_TAG_volatile_type, result);
    if(type->isConstDecoratedType())
        result = backend->getDIBuilder()->createQualifiedType(llvm::dwarf::DW_TAG_const_type, result);

    return wrapLLVMDebugType(result);
}

AnyValuePtr LLVMDebugTypeVisitor::visitPointerType(const PointerTypePtr &type)
{
    auto baseType = backend->translateDIType(type->getBaseType());
    return wrapLLVMDebugType(backend->getDIBuilder()->createPointerType(baseType, uint32_t(type->getMemorySize()*8), uint32_t(type->getMemoryAlignment()*8)));
}

AnyValuePtr LLVMDebugTypeVisitor::visitReferenceType(const ReferenceTypePtr &type)
{
    auto baseType = backend->translateDIType(type->getBaseType());
    return wrapLLVMDebugType(backend->getDIBuilder()->createReferenceType(llvm::dwarf::DW_TAG_reference_type, baseType, uint32_t(type->getMemorySize()*8), uint32_t(type->getMemoryAlignment()*8)));
}

AnyValuePtr LLVMDebugTypeVisitor::visitTemporaryReferenceType(const TemporaryReferenceTypePtr &type)
{
    auto baseType = backend->translateDIType(type->getBaseType());
    return wrapLLVMDebugType(backend->getDIBuilder()->createReferenceType(llvm::dwarf::DW_TAG_rvalue_reference_type, baseType, uint32_t(type->getMemorySize()*8), uint32_t(type->getMemoryAlignment()*8)));
}

AnyValuePtr LLVMDebugTypeVisitor::visitFunctionalType(const FunctionalTypePtr &type)
{
    return wrapLLVMDebugType(backend->getOrCreateDIFunctionType(type));
}

AnyValuePtr LLVMDebugTypeVisitor::visitArrayType(const ArrayTypePtr &type)
{
    auto builder = backend->getDIBuilder();
    auto elementType = backend->translateDIType(type->elementType);
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;

    llvm::Metadata* subscripts[] = {
        builder->getOrCreateSubrange(0, type->size)
    };
    
    return wrapLLVMDebugType(builder->createArrayType(size, uint32_t(alignment), elementType, builder->getOrCreateArray(subscripts)));
}

AnyValuePtr LLVMDebugTypeVisitor::visitPrimitiveVectorType(const PrimitiveVectorTypePtr &type)
{
    auto builder = backend->getDIBuilder();
    auto elementType = backend->translateDIType(type->elementType);
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;

    llvm::Metadata* subscripts[] = {
        builder->getOrCreateSubrange(0, type->elements)
    };
    
    return wrapLLVMDebugType(builder->createVectorType(size, uint32_t(alignment), elementType, builder->getOrCreateArray(subscripts)));
}

llvm::DIType *LLVMDebugTypeVisitor::translateFieldOf(const FieldVariablePtr &field, llvm::DIScope *parent)
{
    auto name = field->getValidNameString();
    auto sourcePosition = field->getDefinitionPosition();
    auto file = backend->getOrCreateDIFileForSourcePosition(sourcePosition);
    const auto &valueType = field->getValueType();
    return backend->getDIBuilder()->createMemberType(
        nullptr, name, file, sourcePosition->getLine(),
        uint32_t(valueType->getMemorySize()*8),
        uint32_t(valueType->getMemoryAlignment()*8),
        field->getOffset()*8, llvm::DINode::FlagPublic,
        backend->translateDIType(valueType)
    );
}

llvm::DIType *LLVMDebugTypeVisitor::translateAggregateTypeWithFields(const AggregateTypeWithFieldsPtr &type, unsigned int tag)
{
    auto sourcePosition = type->getSourceDefinitionPosition();
    auto file = backend->getOrCreateDIFileForSourcePosition(sourcePosition);
    llvm::DIScope* scope = nullptr;

    auto parentProgramEntity = type->getParentProgramEntity();
    if(parentProgramEntity)
        scope = backend->getOrCreateDIScopeForSSAProgramEntity(staticObjectCast<SSAProgramEntity> (parentProgramEntity->asSSAValueRequiredInPosition(ASTSourcePosition::empty())));
    if(!scope)
        scope = file;

    auto builder = backend->getDIBuilder();
    auto name = type->getValidNameStringIncludingTemplateName();
    auto line = sourcePosition->getLine();
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;
    auto declaration = builder->createReplaceableCompositeType(tag,
        name, scope, file, line, 0,
        size, uint32_t(alignment));
    backend->setDebugTypeTranslation(type, declaration);

    std::vector<llvm::Metadata*> elements;
    elements.reserve(type->getFieldCount());
    for(auto &field : type->getFields())
    {
        if(validAnyValue(field->getName())->isHiddenNameSymbol())
            continue;

        elements.push_back(translateFieldOf(field, declaration));
    }

    llvm::DICompositeType *definition = nullptr;
    
    if(type->isStructureType())
    {
        definition = builder->createStructType(
            scope, name, file, line,
            size, uint32_t(alignment),
            llvm::DINode::FlagZero, nullptr, 
            builder->getOrCreateArray(elements),
            0, nullptr,
            backend->getNameMangler()->mangleTypeInfo(type)
        );
    }
    else if(type->isClassType())
    {
        definition = builder->createClassType(
            scope, name, file, line,
            size, uint32_t(alignment), 0,
            llvm::DINode::FlagZero, nullptr,
            builder->getOrCreateArray(elements),
            nullptr, nullptr,
            backend->getNameMangler()->mangleTypeInfo(type)
        );
    }
    else if(type->isUnionType())
    {
        definition = builder->createUnionType(
            scope, name, file, line,
            size, uint32_t(alignment),
            llvm::DINode::FlagZero,
            builder->getOrCreateArray(elements),
            0,
            backend->getNameMangler()->mangleTypeInfo(type)
        );
    }
    
    auto result = builder->replaceTemporary(llvm::TempDINode(declaration), definition);
    backend->setDebugTypeTranslation(type, result);
    builder->retainType(result);
    return result;
}

AnyValuePtr LLVMDebugTypeVisitor::visitStructureType(const StructureTypePtr &type)
{
    return wrapLLVMDebugType(translateAggregateTypeWithFields(type, llvm::dwarf::DW_TAG_structure_type));
}

AnyValuePtr LLVMDebugTypeVisitor::visitClassType(const ClassTypePtr &type)
{
    return wrapLLVMDebugType(translateAggregateTypeWithFields(type, llvm::dwarf::DW_TAG_class_type));
}

AnyValuePtr LLVMDebugTypeVisitor::visitUnionType(const UnionTypePtr &type)
{
    return wrapLLVMDebugType(translateAggregateTypeWithFields(type, llvm::dwarf::DW_TAG_union_type));
}

AnyValuePtr LLVMDebugTypeVisitor::visitTupleType(const TupleTypePtr &type)
{
    (void)type;
    return wrapLLVMDebugType(backend->translateDIType(Type::getUIntPointerType()));
}

AnyValuePtr LLVMDebugTypeVisitor::visitVariantType(const VariantTypePtr &type)
{
    (void)type;
    return wrapLLVMDebugType(backend->translateDIType(Type::getUIntPointerType()));
}

} // End of namespace Environment
} // End of namespace Sysmel
