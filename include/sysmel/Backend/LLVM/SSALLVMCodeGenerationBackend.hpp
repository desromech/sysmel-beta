#ifndef SYSMEL_ENVIRONMENT_SSA_LLVM_CODE_GENERATION_BACKEND_HPP
#define SYSMEL_ENVIRONMENT_SSA_LLVM_CODE_GENERATION_BACKEND_HPP
#pragma once

#include "../../Environment/SSACodeGenerationBackend.hpp"


#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100 4127 4146 4244 4247 4245 4456 4458 4459 4267 4310 4324 4624 4996)
#endif

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/MD5.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Bitcode/BitcodeWriter.h"

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <unordered_map>
#include <unordered_set>

namespace Sysmel
{

namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(NameMangler);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VirtualTable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(IdentifierLookupScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope);
}

namespace LLVM
{

using namespace Environment;

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALLVMCodeGenerationBackend)

/**
 * I am the base interface for a SSA based code generation backend
 */
class SYSMEL_COMPILER_LIB_EXPORT SSALLVMCodeGenerationBackend : public SubtypeOf<SSACodeGenerationBackend, SSALLVMCodeGenerationBackend>
{
public:
    static constexpr char const __typeName__[] = "SSALLVMCodeGenerationBackend";

    virtual bool processAndWriteProgramModule(const ProgramModulePtr &programModule) override;

    llvm::Value *findGlobalValueTranslation(const SSAValuePtr &value);
    void setGlobalValueTranslation(const SSAValuePtr &value, llvm::Value *translatedValue);

    llvm::Value *translateGlobalValue(const SSAValuePtr &value);
    llvm::Type *translateType(const TypePtr &type);
    llvm::Constant *translateVirtualTable(const VirtualTablePtr &vtable);
    void setTypeTranslation(const TypePtr &type, llvm::Type *translatedType);
    bool isSignedIntegerType(const TypePtr &type);
    bool isUnsignedIntegerType(const TypePtr &type);
    void setDebugTypeTranslation(const TypePtr &type, llvm::DIType *translatedType);

    llvm::Constant *translateLiteralValueWithExpectedType(const AnyValuePtr &literal, const TypePtr &expectedType);

    llvm::Constant *internStringConstant(const TypePtr &elementType, const std::string &constant, bool addNullTerminator);
    llvm::Constant *internStringConstantPointer(const TypePtr &elementType, const std::string &constant, bool addNullTerminator);

    llvm::LLVMContext *getContext() const
    {
        return context.get();
    }

    const NameManglerPtr &getNameMangler() const
    {
        return nameMangler;
    }

    llvm::Module *getTargetModule() const
    {
        return targetModule.get();
    }

    llvm::DIBuilder *getDIBuilder()
    {
        return diBuilder.get(); 
    }

    llvm::DICompileUnit *getDICompileUnit()
    {
        return diCompileUnit;
    }

    llvm::DIType *translateDIType(const TypePtr &type);

    llvm::DIFile *getOrCreateDIFileFor(const std::string &filename);
    llvm::DIFile *getOrCreateDIFileForSourcePosition(const ASTSourcePositionPtr &sourcePosition);
    llvm::DISubroutineType *getOrCreateDIFunctionType(const FunctionalTypePtr &functionalType);
    llvm::DILocation *getDILocationFor(const ASTSourcePositionPtr &sourcePosition, llvm::DIScope *scope);

    llvm::DIScope *getOrCreateDIScopeForSSAProgramEntity(const SSAProgramEntityPtr &ssaProgramEntity);
    llvm::DIScope *getOrCreateDIScopeForIdentifierLookupScope(const IdentifierLookupScopePtr &scope);
    llvm::DIScope *getOrCreateDIScopeWithFile(llvm::DIScope *scope, llvm::DIFile *file);

protected:
    void initializePrimitiveTypeMap();
    void initializeDebugInfoBuilding();
    void optimizeModule();
    bool writeOutputOnto(llvm::raw_pwrite_stream &out);

    ProgramModulePtr sourceModule;
    NameManglerPtr nameMangler;
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> targetModule;
    std::unique_ptr<llvm::DIBuilder> diBuilder;
    llvm::DICompileUnit *diCompileUnit = nullptr;
    const llvm::Target* currentTarget = nullptr;
    llvm::TargetMachine *currentTargetMachine = nullptr;
    std::unordered_map<TypePtr, llvm::Type*> typeMap;
    std::unordered_set<TypePtr> signedIntegerTypeSet;
    std::unordered_set<TypePtr> unsignedIntegerTypeSet;
    std::unordered_map<TypePtr, std::function<llvm::DIType*(const TypePtr &type)>> basicDebugTypeConstructors;
    std::unordered_map<TypePtr, llvm::DIType*> debugTypeMap;
    std::unordered_map<std::string, llvm::DIFile*> debugFileMap;
    std::unordered_map<SSAValuePtr, llvm::Value*> globalValueMap;
    std::unordered_map<AnyValuePtr, llvm::DIScope*> diScopeMap;
    std::unordered_map<VirtualTablePtr, llvm::Constant*> translatedVirtualTables;
    std::map<std::pair<llvm::DIScope*, llvm::DIFile*>, llvm::DIScope*> scopesWithExtraFiles;
    std::map<std::pair<uint32_t, std::string>, llvm::Constant*> internedStringConstants;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LLVM_CODE_GENERATION_BACKEND_HPP