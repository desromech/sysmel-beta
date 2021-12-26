#include "Backend/LLVM/LLVMDebugTypeVisitor.hpp"

#include "Environment/ASTSourcePosition.hpp"

#include "Environment/FieldVariable.hpp"
#include "Environment/NameMangler.hpp"

#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"

#include "Environment/AggregateTypeSequentialLayout.hpp"

#include "Environment/StructureType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/TupleType.hpp"

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
    return wrapLLVMDebugType(backend->getDIBuilder()->createPointerType(baseType, type->getMemorySize()*8, type->getMemoryAlignment()*8));
}

AnyValuePtr LLVMDebugTypeVisitor::visitReferenceType(const ReferenceTypePtr &type)
{
    auto baseType = backend->translateDIType(type->getBaseType());
    return wrapLLVMDebugType(backend->getDIBuilder()->createReferenceType(llvm::dwarf::DW_TAG_reference_type, baseType, type->getMemorySize()*8, type->getMemoryAlignment()*8));
}

AnyValuePtr LLVMDebugTypeVisitor::visitTemporaryReferenceType(const TemporaryReferenceTypePtr &type)
{
    auto baseType = backend->translateDIType(type->getBaseType());
    return wrapLLVMDebugType(backend->getDIBuilder()->createReferenceType(llvm::dwarf::DW_TAG_rvalue_reference_type, baseType, type->getMemorySize()*8, type->getMemoryAlignment()*8));
}

AnyValuePtr LLVMDebugTypeVisitor::visitFunctionalType(const FunctionalTypePtr &type)
{
    return wrapLLVMDebugType(backend->getOrCreateDIFunctionType(type));
}

llvm::DIType *LLVMDebugTypeVisitor::translateFieldOf(const FieldVariablePtr &field, llvm::DIScope *parent)
{
    auto name = field->getValidNameString();
    auto sourcePosition = field->getDefinitionPosition();
    auto file = backend->getOrCreateDIFileForSourcePosition(sourcePosition);
    const auto &valueType = field->getValueType();
    return backend->getDIBuilder()->createMemberType(
        nullptr, name, file, sourcePosition->getLine(),
        valueType->getMemorySize()*8,
        valueType->getMemoryAlignment()*8,
        field->getOffset()*8, llvm::DINode::FlagPublic,
        backend->translateDIType(valueType)
    );
}

AnyValuePtr LLVMDebugTypeVisitor::visitStructureType(const StructureTypePtr &type)
{
    auto sourcePosition = type->getSourceDefinitionPosition();
    auto file = backend->getOrCreateDIFileForSourcePosition(sourcePosition);
    auto scope = file;

    auto builder = backend->getDIBuilder();
    auto name = type->getValidNameString();
    auto line = sourcePosition->getLine();
    auto size = type->getMemorySize()*8;
    auto alignment = type->getMemoryAlignment()*8;
    auto declaration = builder->createReplaceableCompositeType(llvm::dwarf::DW_TAG_structure_type,
        name, scope, file, line, 0,
        size, alignment);
    backend->setDebugTypeTranslation(type, declaration);

    std::vector<llvm::Metadata*> elements;
    elements.reserve(type->getFieldCount());
    for(auto &field : type->getFields())
    {
        if(validAnyValue(field->getName())->isHiddenNameSymbol())
            continue;

        elements.push_back(translateFieldOf(field, declaration));
    }

    auto definition = builder->createStructType(
        scope, name, file, line,
        size, alignment,
        llvm::DINode::FlagZero, nullptr, 
        builder->getOrCreateArray(elements),
        0, nullptr,
        backend->getNameMangler()->mangleTypeInfo(type)
    );
    
    auto result = builder->replaceTemporary(llvm::TempDINode(declaration), definition);
    backend->setDebugTypeTranslation(type, result);
    builder->retainType(result);
    return wrapLLVMDebugType(result);
}

} // End of namespace Environment
} // End of namespace Sysmel
