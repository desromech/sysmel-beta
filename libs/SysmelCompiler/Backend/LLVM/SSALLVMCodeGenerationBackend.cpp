#include "Backend/LLVM/SSALLVMCodeGenerationBackend.hpp"
#include "Backend/LLVM/SSALLVMModuleVisitor.hpp"
#include "Backend/LLVM/SSALLVMValueVisitor.hpp"
#include "Backend/LLVM/SSALLVMDebugValueVisitor.hpp"
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

#include "Environment/VirtualTable.hpp"

#include "Environment/RuntimeContext.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/SSAProgramEntity.hpp"

#include <mutex>
#include <fstream>
#include <iostream>

namespace Sysmel
{
namespace Environment
{

std::string getHostTargetTriple()
{
    return llvm::sys::getDefaultTargetTriple();
}

static std::once_flag llvmTargetInitializationFlag;
SSACodeGenerationBackendPtr SSACodeGenerationBackend::makeNativeBackend()
{
    std::call_once(llvmTargetInitializationFlag, [](){
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();
    });

    return basicMakeObject<LLVM::SSALLVMCodeGenerationBackend> ();
}

} // End of namespace Environment

namespace LLVM
{

static BootstrapTypeRegistration<SSALLVMCodeGenerationBackend> SSALLVMCodeGenerationBackendTypeRegistration;

static llvm::CodeGenOpt::Level optimizationLevelToCodeGen(OptimizationLevel level)
{
    switch(level)
    {
    case OptimizationLevel::O3: return llvm::CodeGenOpt::Aggressive;
    default: return llvm::CodeGenOpt::Default;
    }
}

static llvm::PassBuilder::OptimizationLevel convertOptimizationLevel(OptimizationLevel level)
{
    switch (level)
    {
    case OptimizationLevel::O1: return llvm::PassBuilder::OptimizationLevel::O1;
    case OptimizationLevel::O2: return llvm::PassBuilder::OptimizationLevel::O2;
    case OptimizationLevel::O3: return llvm::PassBuilder::OptimizationLevel::O3;
    case OptimizationLevel::Os: return llvm::PassBuilder::OptimizationLevel::Os;
    case OptimizationLevel::Oz: return llvm::PassBuilder::OptimizationLevel::Oz;
    case OptimizationLevel::O0:
    default: return llvm::PassBuilder::OptimizationLevel::O0;
    }
}

typedef llvm::Optional<llvm::DIFile::ChecksumInfo<llvm::StringRef>> DIFileChecksum;
static DIFileChecksum computeChecksumForFileNamed(const std::string &filename)
{
    std::ifstream in(filename);
    if(!in.good())
        return llvm::None;

    auto content = std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());
    llvm::MD5 hash;
    hash.update(content);
    llvm::MD5::MD5Result hashResult;
    hash.final(hashResult);

    auto hexDigitFor = [](uint8_t v) {
        if(v <= 9)
            return char('0' + v);
        else
            return char('A' + v - 10);
    };

    std::string hashResultString;
    hashResultString.reserve(32);
    for(auto b : hashResult.Bytes)
    {
        hashResultString.push_back(hexDigitFor(b >> 4));
        hashResultString.push_back(hexDigitFor(b & 0xf));
    }
    
    return llvm::DIFile::ChecksumInfo<llvm::StringRef>(llvm::DIFile::CSK_MD5, hashResultString);
}

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

        {VirtualTable::__staticType__(), llvm::PointerType::getUnqual(llvm::Type::getInt8PtrTy(*context))},

        {Type::__staticType__(), llvm::Type::getInt8PtrTy(*context)},
    };

    signedIntegerTypeSet = {
        Int8::__staticType__(),
        Int16::__staticType__(),
        Int32::__staticType__(),
        Int64::__staticType__(),
    };

    unsignedIntegerTypeSet = {
        Boolean8::__staticType__(),

        UInt8::__staticType__(),
        UInt16::__staticType__(),
        UInt32::__staticType__(),
        UInt64::__staticType__(),

        Char8::__staticType__(),
        Char16::__staticType__(),
        Char32::__staticType__(),
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

    auto cvarArgs = [=](const TypePtr &){
        return nullptr;
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

        {Type::getCVarArgsType(), cvarArgs},
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

    auto absoluteFileName = makeAbsolutePath(filename);
    if(filename != absoluteFileName)
    {
        it = debugFileMap.find(absoluteFileName);
        if(it != debugFileMap.end())
            return it->second;
    }

    auto name = basename(absoluteFileName);
    auto dir = dirname(absoluteFileName);
    if(dir.empty())
        dir = ".";

    DIFileChecksum checksum = llvm::None;
    if(debugInformationType == DebugInformationType::CodeView)
        checksum = computeChecksumForFileNamed(absoluteFileName);

    auto diFile = diBuilder->createFile(name, dir, checksum);
    debugFileMap.insert({filename, diFile});
    if(filename != absoluteFileName)
        debugFileMap.insert({absoluteFileName, diFile});
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

llvm::Constant *SSALLVMCodeGenerationBackend::translateVirtualTable(const VirtualTablePtr &vtable)
{
    auto it = translatedVirtualTables.find(vtable);
    if(it != translatedVirtualTables.end())
        return it->second;

    auto name = nameMangler->mangleVirtualTable(vtable);
    auto elementType = llvm::Type::getInt8PtrTy(*context);
    auto tableType = llvm::ArrayType::get(elementType, vtable->slots.size());
    auto ownerType = vtable->ownerType.lock();

    // TODO: Compute a proper linkage for this.
    auto linkage = llvm::GlobalValue::ExternalLinkage;
    auto vtableGlobal = new llvm::GlobalVariable(*targetModule, tableType, true, linkage, nullptr, name);

    // Cast and set the vtable result.
    auto resultType = translateType(vtable->getType());
    auto result = llvm::ConstantExpr::getBitCast(vtableGlobal, resultType);
    translatedVirtualTables.insert({vtable, result});

    // Set the vtable elements.
    std::vector<llvm::Constant*> vtableElements;
    vtableElements.reserve(vtable->slots.size());
    for(auto &slot : vtable->slots)
    {
        auto slotSSA = slot->asSSAValueRequiredInPosition(ASTSourcePosition::empty());
        auto slotConstant = llvm::cast<llvm::Constant> (translateGlobalValue(slotSSA));
        
        vtableElements.push_back(llvm::ConstantExpr::getBitCast(slotConstant, elementType));
    }

    vtableGlobal->setInitializer(llvm::ConstantArray::get(tableType, vtableElements));

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

llvm::DIScope *SSALLVMCodeGenerationBackend::getOrCreateDIScopeForSSAProgramEntity(const SSAProgramEntityPtr &ssaProgramEntity)
{
    if(!ssaProgramEntity)
        return nullptr;

    auto it = diScopeMap.find(ssaProgramEntity);
    if(it != diScopeMap.end())
        return it->second;

    auto visitor = basicMakeObject<SSALLVMDebugValueVisitor> ();
    visitor->backend = this;
    auto result = visitor->translateDIScope(ssaProgramEntity);
    diScopeMap.insert({ssaProgramEntity, result});
    return result;
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
    auto elementSize = elementType->getMemorySize();
    {
        auto it = internedStringConstants.find({elementSize, constant});
        if(it != internedStringConstants.end())
            return it->second;
    }

    auto stringConstantData = internStringConstant(elementType, constant, addNullTerminator);
    auto result = new llvm::GlobalVariable(*targetModule, stringConstantData->getType(), true, llvm::GlobalValue::PrivateLinkage, stringConstantData);
    internedStringConstants.insert({{elementSize, constant}, result});
    return result;
}

bool SSALLVMCodeGenerationBackend::isSignedIntegerType(const TypePtr &type)
{
    return signedIntegerTypeSet.find(type->asUndecoratedType()) != signedIntegerTypeSet.end();
}

bool SSALLVMCodeGenerationBackend::isUnsignedIntegerType(const TypePtr &type)
{
    return unsignedIntegerTypeSet.find(type->asUndecoratedType()) != unsignedIntegerTypeSet.end();
}

void SSALLVMCodeGenerationBackend::optimizeModule()
{
    // See https://llvm.org/docs/NewPassManager.html
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;

    llvm::PassBuilder PB;

    FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });

    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    llvm::ModulePassManager MPM;
    if(optimizationLevel == OptimizationLevel::O0)
        MPM.addPass(createModuleToFunctionPassAdaptor(llvm::SimplifyCFGPass()));
    else
        MPM = PB.buildPerModuleDefaultPipeline(convertOptimizationLevel(optimizationLevel));

    MPM.run(*targetModule, MAM);
}

bool SSALLVMCodeGenerationBackend::processAndWriteProgramModule(const ProgramModulePtr &programModule)
{
    sourceModule = programModule;
    context = std::make_unique<llvm::LLVMContext> ();
    targetModule = std::make_unique<llvm::Module> (validAnyValue(programModule->getName())->asString(), *context);
    nameMangler = basicMakeObject<ItaniumNameMangler> ();

    auto &currentTargetDescription = RuntimeContext::getActive()->getTargetDescription();
    auto &currentTargetNameAndFeatures = currentTargetDescription.nameAndFeatures;
    auto &triple = currentTargetNameAndFeatures.triple;
    targetModule->setTargetTriple(triple);

    // Use codeview for the default debug information type on windows.
    if(currentTargetNameAndFeatures.isWindows())
    {
        if(debugInformationType == DebugInformationType::Default)
            debugInformationType = DebugInformationType::CodeView;
    }

    // Lookup the LLVM target.
    {
        std::string error;
        currentTarget = llvm::TargetRegistry::lookupTarget(triple, error);
        if(!emitTargetIR && !currentTarget)
        {
            std::cerr << "Failed to find llvm target: " << error << std::endl;
            return false;
        }

    }

    auto isPie = (picMode == PICMode::Default && currentTargetNameAndFeatures.defaultsToPIE()) || picMode == PICMode::PIE;
    auto isPic = picMode == PICMode::PIC;

    // Position independent code
    if(isPic) targetModule->setPICLevel(llvm::PICLevel::BigPIC);
    if(isPie) targetModule->setPIELevel(llvm::PIELevel::Large);

    // Create the target machine.
    if(currentTarget)
    {
        auto cpu = currentTargetNameAndFeatures.cpu;
        auto features = "";

        llvm::TargetOptions opt;

        // Default to separate sections.
        opt.FunctionSections = true;
        opt.DataSections = true;

        // Convert the relocation model.
        auto relocationModel = llvm::Optional<llvm::Reloc::Model>();
        if(isPic || isPie)
            relocationModel = llvm::Reloc::Model::PIC_;

        // Float abi.
        if(currentTargetNameAndFeatures.floatAbi == "soft")
            opt.FloatABIType = llvm::FloatABI::Soft;
        else if(currentTargetNameAndFeatures.floatAbi == "hard")
            opt.FloatABIType = llvm::FloatABI::Hard;

        currentTargetMachine = currentTarget->createTargetMachine(triple, cpu, features, opt, relocationModel);
        targetModule->setDataLayout(currentTargetMachine->createDataLayout());

        currentTargetMachine->setOptLevel(optimizationLevelToCodeGen(optimizationLevel));
    }

    // Initialize the type map.
    initializePrimitiveTypeMap();
    initializeDebugInfoBuilding();

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

    // Optimize the module.
    optimizeModule();

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

bool SSALLVMCodeGenerationBackend::writeOutputOnto(llvm::raw_pwrite_stream &out)
{
    if(emitTargetIR)
    {
        switch(outputMode)
        {
        case SSACodeGenerationOutputMode::Assembly:
            targetModule->print(out, nullptr);
            return true;
        case SSACodeGenerationOutputMode::ObjectFile:
            llvm::WriteBitcodeToFile(*targetModule, out);
            return true;
        default:
            std::cerr << "Invalid output mode for writing the LLVM IR." << std::endl;
            return false;
        }
    }

    if(!currentTargetMachine)
    {
        std::cerr << "Cannot write output file without a LLVM target machine." << std::endl;
        return false;
    }

    auto codeGenFileType = outputMode == SSACodeGenerationOutputMode::Assembly
        ? llvm::CGFT_AssemblyFile
        : llvm::CGFT_ObjectFile;

    llvm::legacy::PassManager outputGenerationPass;
    currentTargetMachine->addPassesToEmitFile(outputGenerationPass, out, nullptr, codeGenFileType);

    switch(outputMode)
    {
    case SSACodeGenerationOutputMode::Assembly:
    case SSACodeGenerationOutputMode::ObjectFile:
    case SSACodeGenerationOutputMode::Executable:
    case SSACodeGenerationOutputMode::SharedLibrary:
    case SSACodeGenerationOutputMode::Plugin:
        outputGenerationPass.run(*targetModule);
        return true;
    default:
        std::cerr << "Unsupported output mode with the llvm backend." << std::endl;
        return false;
    }
}
} // End of namespace Environment
} // End of namespace Sysmel