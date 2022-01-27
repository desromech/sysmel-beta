#include "Backend/LLVM/LLVMDebugTypeVisitor.hpp"

#include "Environment/ASTSourcePosition.hpp"

#include "Environment/FieldVariable.hpp"
#include "Environment/NameMangler.hpp"

#include "Environment/MetaType.hpp"

#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/ClosureType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"

#include "Environment/ArrayType.hpp"
#include "Environment/PrimitiveVectorType.hpp"

#include "Environment/AggregateTypeVariantLayout.hpp"
#include "Environment/AggregateTypeSequentialLayout.hpp"

#include "Environment/StructureType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/VariantType.hpp"

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

AnyValuePtr LLVMDebugTypeVisitor::visitEnumType(const EnumTypePtr &type)
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

    auto baseType = backend->translateDIType(type->getBaseType());

    std::vector<llvm::Metadata*> elements;

    auto enumType = backend->getDIBuilder()->createEnumerationType(
        scope, name, file, line,
        size, alignment,
        builder->getOrCreateArray(elements), baseType,
        backend->getNameMangler()->mangleTypeInfo(type)
    );
    return wrapLLVMDebugType(enumType);
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

AnyValuePtr LLVMDebugTypeVisitor::visitClosureType(const ClosureTypePtr &type)
{
    auto name = type->getValidNameStringIncludingTemplateName();
    auto builder = backend->getDIBuilder();
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;
    auto closureType = builder->createStructType(
        nullptr, name, nullptr, 0,
        size, uint32_t(alignment),
        llvm::DINode::FlagTypePassByReference, nullptr, 
        builder->getOrCreateArray({}),
        0, nullptr,
        backend->getNameMangler()->mangleTypeInfo(type)
    );

    auto closurePointerType = builder->createPointerType(closureType, 
        type->getMemorySize()*8, type->getMemoryAlignment()*8);
    backend->setDebugTypeTranslation(type, closurePointerType);

    //auto closureFunctionType = backend->getOrCreateDIFunctionType(type);
    return wrapLLVMDebugType(closurePointerType);
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
        parent, name, file, sourcePosition->getLine(),
        uint64_t(valueType->getMemorySize()*8),
        uint32_t(valueType->getMemoryAlignment()*8),
        field->getOffset()*8, llvm::DINode::FlagPublic,
        backend->translateDIType(valueType)
    );
}

llvm::DIType *LLVMDebugTypeVisitor::translateAggregateTypeWithFields(const AggregateTypeWithFieldsPtr &type, unsigned int tag)
{
    (void)tag;
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

    llvm::DICompositeType *result = nullptr;
    if(type->isStructureType())
    {
        result = builder->createStructType(
            scope, name, file, line,
            size, uint32_t(alignment),
            llvm::DINode::FlagZero, nullptr, 
            builder->getOrCreateArray({}),
            0, nullptr,
            backend->getNameMangler()->mangleTypeInfo(type)
        );
    }
    else if(type->isClassType())
    {
        result = builder->createClassType(
            scope, name, file, line,
            size, uint32_t(alignment), 0,
            llvm::DINode::FlagZero, nullptr,
            builder->getOrCreateArray({}),
            nullptr, nullptr,
            backend->getNameMangler()->mangleTypeInfo(type)
        );
    }
    else if(type->isUnionType())
    {
        result = builder->createUnionType(
            scope, name, file, line,
            size, uint32_t(alignment),
            llvm::DINode::FlagZero,
            builder->getOrCreateArray({}),
            0,
            backend->getNameMangler()->mangleTypeInfo(type)
        );
    }
    backend->setDebugTypeTranslation(type, result);
    
    std::vector<llvm::Metadata*> elements;
    elements.reserve(type->getFieldCount());
    for(auto &field : type->getFields())
    {
        if(validAnyValue(field->getName())->isHiddenNameSymbol())
            continue;

        elements.push_back(translateFieldOf(field, result));
    }


    builder->replaceArrays(result, builder->getOrCreateArray(elements));
    backend->setDebugTypeTranslation(type, result);
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
    auto builder = backend->getDIBuilder();

    auto name = type->getQualifiedName();
    auto line = 0;
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;
    auto tupleType = builder->createStructType(
        nullptr, name, nullptr, line,
        size, uint32_t(alignment),
        llvm::DINode::FlagTypePassByReference, nullptr, 
        builder->getOrCreateArray({}),
        0, nullptr,
        backend->getNameMangler()->mangleTypeInfo(type)
    );
    backend->setDebugTypeTranslation(type, tupleType);

    return wrapLLVMDebugType(tupleType);
}

AnyValuePtr LLVMDebugTypeVisitor::visitVariantType(const VariantTypePtr &type)
{
    auto builder = backend->getDIBuilder();

    auto layout = type->getLayout().staticAs<AggregateTypeVariantLayout> ();

    auto scope = nullptr;
    auto file = nullptr;
    auto line = 0;

    auto name = type->getQualifiedName();
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;

    auto discriminantOffset = layout->getOffsetForSlotIndex(0)*8;
    auto elementOffset = layout->getOffsetForSlotIndex(layout->getElementMemorySlotIndex())*8;

    auto discriminatorType = layout->getDataTypeIndexType();
    auto discriminatorLLVMType = backend->translateType(discriminatorType);
    auto discriminator = builder->createMemberType(
        nullptr, "typeSelector", nullptr, 0,
        discriminatorType->getMemorySize()*8, discriminatorType->getMemoryAlignment()*8,
        discriminantOffset, llvm::DINode::FlagArtificial, backend->translateDIType(discriminatorType)
    );

    auto variantType = builder->createStructType(
        scope, name, file, line,
        size, uint32_t(alignment),
        llvm::DINode::FlagTypePassByReference, nullptr, 
        builder->getOrCreateArray({}),
        0, nullptr,
        backend->getNameMangler()->mangleTypeInfo(type)
    );
    backend->setDebugTypeTranslation(type, variantType);

    auto variantPart = builder->createVariantPart(
        variantType, "", nullptr, 0,
        size, uint32_t(alignment),
        llvm::DINode::FlagZero, discriminator, builder->getOrCreateArray({}),
        backend->getNameMangler()->mangleTypeInfo(type)
    );

    std::vector<llvm::Metadata*> elements;
    elements.reserve(type->elementTypes.size());
    for(size_t i = 0; i < type->elementTypes.size(); ++i)
    {
        auto discriminantConstant = llvm::ConstantInt::get(discriminatorLLVMType, i);

        auto elementType = type->elementTypes[i];
        auto elementDebugType = backend->translateDIType(elementType);
        auto elementName = "";
        elements.push_back(builder->createVariantMemberType(variantPart, elementName, nullptr, 0,
            elementType->getMemorySize()*8, elementType->getMemoryAlignment()*8, elementOffset,
            discriminantConstant, llvm::DINode::FlagZero, elementDebugType
        ));
    }

    builder->replaceArrays(variantPart, builder->getOrCreateArray(elements));

    llvm::Metadata* typeElements[] = {variantPart};

    builder->replaceArrays(variantType, builder->getOrCreateArray(typeElements));
    
    return wrapLLVMDebugType(variantType);
}

} // End of namespace Environment
} // End of namespace Sysmel
