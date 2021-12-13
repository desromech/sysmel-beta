#ifndef SYSMEL_ENVIRONMENT_SSA_LLVM_CODE_GENERATION_BACKEND_HPP
#define SYSMEL_ENVIRONMENT_SSA_LLVM_CODE_GENERATION_BACKEND_HPP
#pragma once

#include "../../Environment/SSACodeGenerationBackend.hpp"


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"


#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

namespace Sysmel
{
namespace LLVM
{

using namespace Environment;

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALLVMCodeGenerationBackend)

/**
 * I am the base interface for a SSA based code generation backend
 */
class SSALLVMCodeGenerationBackend : public SubtypeOf<SSACodeGenerationBackend, SSALLVMCodeGenerationBackend>
{
public:
    static constexpr char const __typeName__[] = "SSALLVMCodeGenerationBackend";

    virtual bool processAndWriteProgramModule(const ProgramModulePtr &programModule) override;

    llvm::Value *findGlobalValueTranslation(const SSAValuePtr &value);
    void setGlobalValueTranslation(const SSAValuePtr &value, llvm::Value *translatedValue);

    llvm::Value *translateGlobalValue(const SSAValuePtr &value);
    llvm::Type *translateType(const TypePtr &type);
    bool isSignedIntegerType(const TypePtr &type);

    llvm::LLVMContext *getContext() const
    {
        return context.get();
    }

    llvm::Module *getTargetModule() const
    {
        return targetModule.get();
    }

protected:
    void initializePrimitiveTypeMap();
    bool writeOutputOnto(llvm::raw_ostream &out);


    ProgramModulePtr sourceModule;
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> targetModule;
    std::unordered_map<TypePtr, llvm::Type*> typeMap;
    std::unordered_set<TypePtr> signedIntegerTypeSet;
    std::unordered_map<SSAValuePtr, llvm::Value*> globalValueMap;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LLVM_CODE_GENERATION_BACKEND_HPP