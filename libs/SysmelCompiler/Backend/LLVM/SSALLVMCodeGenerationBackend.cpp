#include "Backend/LLVM/SSALLVMCodeGenerationBackend.hpp"
#include "Backend/LLVM/SSALLVMModuleVisitor.hpp"
#include "Backend/LLVM/SSALLVMValueVisitor.hpp"
#include "Backend/LLVM/LLVMLiteralValueVisitor.hpp"
#include "Backend/LLVM/LLVMDebugTypeVisitor.hpp"
#include "Backend/LLVM/LLVMTypeVisitor.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/FunctionType.hpp"

#include "Environment/ItaniumNameMangler.hpp"

#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/StringUtilities.hpp"

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
    typeMap = {
        {Type::getVoidType(), llvm::Type::getVoidTy(*context)},
        {Type::getControlFlowEscapeType(), llvm::Type::getVoidTy(*context)},
        {Type::getReturnType(), llvm::Type::getVoidTy(*context)},
        {Type::getBreakType(), llvm::Type::getVoidTy(*context)},
        {Type::getContinueType(), llvm::Type::getVoidTy(*context)},

        {UInt8::__staticType__(), llvm::Type::getInt8Ty(*context)},
        {Int8::__staticType__(), llvm::Type::getInt8Ty(*context)},
        {Char8::__staticType__(), llvm::Type::getInt8Ty(*context)},
        {Boolean8::__staticType__(), llvm::Type::getInt8Ty(*context)},

        {UInt16::__staticType__(), llvm::Type::getInt16Ty(*context)},
        {Int16::__staticType__(), llvm::Type::getInt16Ty(*context)},
        {Char16::__staticType__(), llvm::Type::getInt16Ty(*context)},

        {UInt32::__staticType__(), llvm::Type::getInt32Ty(*context)},
        {Int32::__staticType__(), llvm::Type::getInt32Ty(*context)},
        {Char32::__staticType__(), llvm::Type::getInt32Ty(*context)},

        {UInt64::__staticType__(), llvm::Type::getInt64Ty(*context)},
        {Int64::__staticType__(), llvm::Type::getInt64Ty(*context)},

        {Float16::__staticType__(), llvm::Type::getHalfTy(*context)},
        {Float32::__staticType__(), llvm::Type::getFloatTy(*context)},
        {Float64::__staticType__(), llvm::Type::getDoubleTy(*context)},

        {Type::__staticType__(), llvm::Type::getInt8PtrTy(*context)},
    };

    signedIntegerTypeSet = {
        Int8::__staticType__(),
        Int16::__staticType__(),
        Int32::__staticType__(),
        Int64::__staticType__(),
    };
}

void SSALLVMCodeGenerationBackend::initializeDebugInfoBuilding()
{
    if(debugInformationType == DebugInformationType::None)
        return;

    diBuilder = std::make_unique<llvm::DIBuilder> (*targetModule);
    diCompileUnit = diBuilder->createCompileUnit(llvm::dwarf::DW_LANG_C_plus_plus, getOrCreateDIFileFor(mainInputFileName), "Sysmel Compiler", false, "", 0);

    auto primitiveVoidType = [=](const TypePtr &){
        return nullptr;
    };

    auto primitiveBoolean = [=](const TypePtr &type){
        return diBuilder->createBasicType(type->getValidNameString(), type->getMemorySize()*8, llvm::dwarf::DW_ATE_boolean);
    };

    auto primitiveUnsignedInt = [=](const TypePtr &type){
        return diBuilder->createBasicType(type->getValidNameString(), type->getMemorySize()*8, llvm::dwarf::DW_ATE_unsigned);
    };

    auto primitiveSignedInt = [=](const TypePtr &type){
        return diBuilder->createBasicType(type->getValidNameString(), type->getMemorySize()*8, llvm::dwarf::DW_ATE_signed);
    };

    auto primitiveCharacter = [=](const TypePtr &type){
        return diBuilder->createBasicType(type->getValidNameString(), type->getMemorySize()*8, llvm::dwarf::DW_ATE_unsigned_char);
    };

    auto primitiveFloat = [=](const TypePtr &type){
        return diBuilder->createBasicType(type->getValidNameString(), type->getMemorySize()*8, llvm::dwarf::DW_ATE_float);
    };

    basicDebugTypeConstructors = {
        {Type::getVoidType(), primitiveVoidType},
        {Type::getControlFlowEscapeType(), primitiveVoidType},
        {Type::getReturnType(), primitiveVoidType},
        {Type::getBreakType(), primitiveVoidType},
        {Type::getContinueType(), primitiveVoidType},

        {Boolean8::__staticType__(), primitiveBoolean},

        {UInt8::__staticType__(), primitiveUnsignedInt},
        {UInt16::__staticType__(), primitiveUnsignedInt},
        {UInt32::__staticType__(), primitiveUnsignedInt},
        {UInt64::__staticType__(), primitiveUnsignedInt},

        {Int8::__staticType__(), primitiveSignedInt},
        {Int16::__staticType__(), primitiveSignedInt},
        {Int32::__staticType__(), primitiveSignedInt},
        {Int64::__staticType__(), primitiveSignedInt},

        {Char8::__staticType__(), primitiveCharacter},
        {Char16::__staticType__(), primitiveCharacter},
        {Char32::__staticType__(), primitiveCharacter},

        {Float16::__staticType__(), primitiveFloat},
        {Float32::__staticType__(), primitiveFloat},
        {Float64::__staticType__(), primitiveFloat},
    };
}

llvm::DIType *SSALLVMCodeGenerationBackend::translateDIType(const TypePtr &type)
{
    // Check already translated debug type map.
    {
        auto it = debugTypeMap.find(type);
        if(it != debugTypeMap.end())
            return it->second;
    }

    // Check for lazy basic type.
    {
        auto it = basicDebugTypeConstructors.find(type);
        if(it != basicDebugTypeConstructors.end())
        {
            auto translatedType = it->second(type);
            debugTypeMap.insert({type, translatedType});
            return translatedType;
        }
    }

    // Visit the type.
    auto visitor = basicMakeObject<LLVMDebugTypeVisitor> ();
    visitor->backend = this;
    
    auto result = visitor->visitType(type);
    sysmelAssert(result);
    debugTypeMap.insert({type, result});
    return result;
}

llvm::DIFile *SSALLVMCodeGenerationBackend::getOrCreateDIFileFor(const std::string &filename)
{
    if(!diBuilder)
        return nullptr;

    auto it = debugFileMap.find(filename);
    if(it != debugFileMap.end())
        return it->second;

    auto name = basename(filename);
    auto dir = dirname(filename);
    if(dir.empty())
        dir = ".";

    auto diFile = diBuilder->createFile(name, dir);
    debugFileMap.insert({filename, diFile});
    return diFile;
}

llvm::DIFile *SSALLVMCodeGenerationBackend::getOrCreateDIFileForSourcePosition(const ASTSourcePositionPtr &sourcePosition)
{
    return getOrCreateDIFileFor(sourcePosition->getSourceCodeFileName());
}

llvm::DISubroutineType *SSALLVMCodeGenerationBackend::getOrCreateDIFunctionType(const FunctionalTypePtr &functionalType)
{
    sysmelAssert(diBuilder);
    {
        auto it = debugTypeMap.find(functionalType);
        if(it != debugTypeMap.end())
            return llvm::cast<llvm::DISubroutineType> (it->second);
    }

    std::vector<llvm::Metadata*> elementTypes;
    auto argumentCount = functionalType->getArgumentCount();
    elementTypes.reserve(2 + argumentCount);
    
    // Add the result type.
    elementTypes.push_back(translateDIType(functionalType->getResultType()));

    // Add the receiver type.
    auto receiverType = functionalType->getReceiverType();
    if(!receiverType->isVoidType())
        elementTypes.push_back(translateDIType(receiverType));

    // Add the argument type.
    for(size_t i = 0; i < argumentCount; ++i)
        elementTypes.push_back(translateDIType(functionalType->getArgument(i)));

    auto functionType = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(elementTypes));
    debugTypeMap.insert({functionalType, functionType});

    return functionType;
}

llvm::DILocation *SSALLVMCodeGenerationBackend::getDILocationFor(const ASTSourcePositionPtr &sourcePosition, llvm::DIScope *scope)
{
    if(!sourcePosition || sourcePosition->isASTEmptySourcePosition())
        return nullptr;

    return llvm::DILocation::get(*context, sourcePosition->getLine(), sourcePosition->getColumn(), scope ? scope : diCompileUnit);
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
    sysmelAssert(result);
    typeMap.insert({type, result});
    return result;
}

void SSALLVMCodeGenerationBackend::setTypeTranslation(const TypePtr &type, llvm::Type *translatedType)
{
    typeMap.insert({type, translatedType});
}

void SSALLVMCodeGenerationBackend::setDebugTypeTranslation(const TypePtr &type, llvm::DIType *translatedType)
{
    debugTypeMap[type] = translatedType;
}

llvm::Constant *SSALLVMCodeGenerationBackend::translateLiteralValueWithExpectedType(const AnyValuePtr &literal, const TypePtr &expectedType)
{
    auto visitor = basicMakeObject<LLVMLiteralValueVisitor> ();
    visitor->backend = this;
    return visitor->translateValueWithExpectedType(literal, expectedType);
}

llvm::Constant *SSALLVMCodeGenerationBackend::internStringConstant(const TypePtr &elementType, const std::string &constant, bool addNullTerminator)
{
    switch(elementType->getMemorySize())
    {
    case 1:
        return llvm::ConstantDataArray::getString(*context, constant, addNullTerminator);
    case 2:
    case 4:
    default:
        sysmelAssert("Unsupported string type." && false);
    }
}

llvm::Constant *SSALLVMCodeGenerationBackend::internStringConstantPointer(const TypePtr &elementType, const std::string &constant, bool addNullTerminator)
{
    auto stringConstantData = internStringConstant(elementType, constant, addNullTerminator);
    return new llvm::GlobalVariable(*targetModule, stringConstantData->getType(), true, llvm::GlobalValue::PrivateLinkage, stringConstantData);
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
    nameMangler = basicMakeObject<ItaniumNameMangler> ();

    // Initialize the type map.
    initializePrimitiveTypeMap();
    initializeDebugInfoBuilding();

    functionPassManager = std::make_unique<llvm::legacy::FunctionPassManager> (targetModule.get());
    functionPassManager->add(llvm::createCFGSimplificationPass());
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

    // Finalize the debug info
    if(diBuilder)
    {
        diBuilder->finalize();
        targetModule->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);

        switch(debugInformationType)
        {
        case DebugInformationType::Dwarf:
            targetModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 1);
            break;
        case DebugInformationType::Dwarf2:
            targetModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 2);
            break;
        case DebugInformationType::Dwarf3:
            targetModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 3);
            break;
        case DebugInformationType::Dwarf4:
        case DebugInformationType::Default:
            targetModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 4);
            break;
        case DebugInformationType::CodeView:
            targetModule->addModuleFlag(llvm::Module::Warning, "CodeView", 1);
            break;
        default:
            break;
        }
    }

    // Verify the module
    if(llvm::verifyModule(*targetModule, &llvm::errs()))
        abort();

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