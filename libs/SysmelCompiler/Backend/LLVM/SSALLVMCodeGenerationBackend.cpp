#include "Backend/LLVM/SSALLVMCodeGenerationBackend.hpp"
#include "Backend/LLVM/SSALLVMModuleVisitor.hpp"
#include "Backend/LLVM/SSALLVMValueVisitor.hpp"
#include "Backend/LLVM/LLVMTypeVisitor.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/FunctionType.hpp"

#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include <iostream>

namespace Sysmel
{
namespace Environment
{

SSACodeGenerationBackendPtr SSACodeGenerationBackend::makeNativeBackend()
{
    return basicMakeObject<LLVM::SSALLVMCodeGenerationBackend> ();
}

} // End of namespace Environment

namespace LLVM
{

static BootstrapTypeRegistration<SSALLVMCodeGenerationBackend> SSALLVMCodeGenerationBackendTypeRegistration;

void SSALLVMCodeGenerationBackend::initializePrimitiveTypeMap()
{
    typeMap.insert({Type::getVoidType(), llvm::Type::getVoidTy(*context)});
    typeMap.insert({Type::getControlFlowEscapeType(), llvm::Type::getVoidTy(*context)});
    typeMap.insert({Type::getReturnType(), llvm::Type::getVoidTy(*context)});
    typeMap.insert({Type::getBreakType(), llvm::Type::getVoidTy(*context)});
    typeMap.insert({Type::getContinueType(), llvm::Type::getVoidTy(*context)});

    typeMap.insert({UInt8::__staticType__(), llvm::Type::getInt8Ty(*context)});
    typeMap.insert({Int8::__staticType__(), llvm::Type::getInt8Ty(*context)});
    typeMap.insert({Char8::__staticType__(), llvm::Type::getInt8Ty(*context)});
    typeMap.insert({Boolean8::__staticType__(), llvm::Type::getInt8Ty(*context)});

    typeMap.insert({UInt16::__staticType__(), llvm::Type::getInt16Ty(*context)});
    typeMap.insert({Int16::__staticType__(), llvm::Type::getInt16Ty(*context)});
    typeMap.insert({Char16::__staticType__(), llvm::Type::getInt16Ty(*context)});

    typeMap.insert({UInt32::__staticType__(), llvm::Type::getInt32Ty(*context)});
    typeMap.insert({Int32::__staticType__(), llvm::Type::getInt32Ty(*context)});
    typeMap.insert({Char32::__staticType__(), llvm::Type::getInt32Ty(*context)});

    typeMap.insert({UInt64::__staticType__(), llvm::Type::getInt64Ty(*context)});
    typeMap.insert({Int64::__staticType__(), llvm::Type::getInt64Ty(*context)});

    typeMap.insert({Float16::__staticType__(), llvm::Type::getHalfTy(*context)});
    typeMap.insert({Float32::__staticType__(), llvm::Type::getFloatTy(*context)});
    typeMap.insert({Float64::__staticType__(), llvm::Type::getDoubleTy(*context)});

    typeMap.insert({Type::__staticType__(), llvm::Type::getInt8PtrTy(*context)});

    signedIntegerTypeSet.insert(Int8::__staticType__());
    signedIntegerTypeSet.insert(Int16::__staticType__());
    signedIntegerTypeSet.insert(Int32::__staticType__());
    signedIntegerTypeSet.insert(Int64::__staticType__());
}

llvm::Value *SSALLVMCodeGenerationBackend::findGlobalValueTranslation(const SSAValuePtr &value)
{
    auto it = globalValueMap.find(value);
    return it != globalValueMap.end() ? it->second : nullptr;
}

void SSALLVMCodeGenerationBackend::setGlobalValueTranslation(const SSAValuePtr &value, llvm::Value *translatedValue)
{
    globalValueMap.insert({value, translatedValue});
}

llvm::Value *SSALLVMCodeGenerationBackend::translateGlobalValue(const SSAValuePtr &value)
{
    auto it = globalValueMap.find(value);
    if(it != globalValueMap.end())
        return it->second;

    auto visitor = basicMakeObject<SSALLVMValueVisitor> ();
    visitor->backend = this;
    return visitor->visitValueForLLVM(value);
}

llvm::Type *SSALLVMCodeGenerationBackend::translateType(const TypePtr &type)
{
    auto it = typeMap.find(type);
    if(it != typeMap.end())
        return it->second;

    auto visitor = basicMakeObject<LLVMTypeVisitor> ();
    visitor->backend = this;
    
    auto result = visitor->visitType(type);
    assert(result);
    typeMap.insert({type, result});
    return result;
}

bool SSALLVMCodeGenerationBackend::isSignedIntegerType(const TypePtr &type)
{
    return signedIntegerTypeSet.find(type) != signedIntegerTypeSet.end();
}

bool SSALLVMCodeGenerationBackend::processAndWriteProgramModule(const ProgramModulePtr &programModule)
{
    sourceModule = programModule;
    context = std::make_unique<llvm::LLVMContext> ();
    targetModule = std::make_unique<llvm::Module> (validAnyValue(programModule->getName())->asString(), *context);

    // Initialize the type map.
    initializePrimitiveTypeMap();

    functionPassManager = std::make_unique<llvm::legacy::FunctionPassManager> (targetModule.get());
    /*functionPassManager->add(llvm::createPromoteMemoryToRegisterPass());
    functionPassManager->add(llvm::createInstructionCombiningPass());
    functionPassManager->add(llvm::createReassociatePass());
    functionPassManager->add(llvm::createGVNPass());
    functionPassManager->add(llvm::createCFGSimplificationPass());*/
    functionPassManager->doInitialization();

    // Process the module components.
    {
        auto visitor = basicMakeObject<SSALLVMModuleVisitor> ();
        visitor->backend = this;
        visitor->visitValue(programModule->asSSAValueRequiredInPosition(ASTSourcePosition::empty()));
    }

    // Emit the module output.
    if(outputFileName.empty() || outputFileName == "-")
    {
        return writeOutputOnto(llvm::outs());
    }
    else
    {
        std::error_code errorCode;
        if(errorCode)
            return false;

        llvm::raw_fd_ostream out(outputFileName, errorCode);
        return writeOutputOnto(out);
    }
}

bool SSALLVMCodeGenerationBackend::writeOutputOnto(llvm::raw_ostream &out)
{
    targetModule->print(out, nullptr);
    return true;
}
} // End of namespace Environment
} // End of namespace Sysmel