#include "Frontend/SysmelSyntax/SysmelLanguageSupport.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSAValue.hpp"
#include "Environment/SSACodeGenerationBackend.hpp"
#include "Environment/StringUtilities.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace Sysmel::Environment;

struct CompilerParameters
{
    bool emitTargetIR = false;
    bool emitSExpression = false;
    bool emitSSASExpression = false;
    bool semanticAnalysisOnly = false;
    TargetNameAndFeatures targetName = TargetNameAndFeatures::getForHost();
    SSACodeGenerationOutputMode outputMode = SSACodeGenerationOutputMode::Executable;
    DebugInformationType debugInformationType = DebugInformationType::None;
    OptimizationLevel optimizationLevel = OptimizationLevel::O0;
    bool useFastMath = false;
    PICMode picMode = PICMode::Default;
    std::string moduleName;
    std::string outputFileName;
    std::vector<std::string> modulePaths;
    std::vector<std::string> importedModules;
    std::vector<std::string> inputFileNames;
};

bool processInputFileNamed(const std::string &fileName)
{
    bool success = true;
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        language->evaluateFileNamed(fileName);
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << "\n";
        success = false;
    }

    return success;
}

void writeStringToOutputFileNamed(const std::string &string, const std::string &outputFileName)
{
    if(outputFileName == "-")
    {
        std::cout << string;
    }
    else
    {
        std::ofstream out(outputFileName, std::ios::trunc | std::ios::binary);
        out << string;
    }

}

void printHelp()
{
    std::cout <<
R"(sysmelc [options] (input files)+
-h -help                    Print this message.
-version                    Print the version info.
-c                          Emit object file.
-S                          Emit assembly.
-shared                     Emit shared library
-emit-target-ir             
-emit-llvm                  Emit target specific ir. Must use one of -c|-S
-module <name>              The name of the module
-o<output name>
-g                          Emit debug information in the default format.
-gdwarf
-gdwarf2
-gdwarf3
-gdwarf4
-gcodeview
-target <triple>
-mcpu <name>
-mfpu <name>
-mfloat-abi <triple>
-O0                         No optimization level
-O1
-O2
-O3
-Os
-Oz
-ffast-math
-fPIC
-fPIE
-semantic-analysis-only     No generation. For error detection and performance measurements.
)" << std::endl;
}

void printVersion()
{
    std::cout << "Sysmel Compiler (Beta Branch)" << std::endl;
}

int main(int argc, const char *argv[])
{
    CompilerParameters parameters;

    for(int i = 1; i < argc; ++i)
    {
        auto arg = std::string(argv[i]);
        if(arg.size() > 1 && arg[0] == '-')
        {
            if(arg == "-h" || arg == "-help")
            {
                printHelp();
                return 0;
            }
            if(arg == "-version")
            {
                printVersion();
                return 0;
            }
            if(arg == "-c")
                parameters.outputMode = SSACodeGenerationOutputMode::ObjectFile;
            else if(arg == "-S")
                parameters.outputMode = SSACodeGenerationOutputMode::Assembly;
            else if(arg == "-shared")
                parameters.outputMode = SSACodeGenerationOutputMode::SharedLibrary;
            else if(arg == "-emit-target-ir" || arg == "-emit-llvm")
                parameters.emitTargetIR = true;
            else if(arg == "-emit-sexpr")
                parameters.emitSExpression = true;
            else if(arg == "-emit-ssa-sexpr")
                parameters.emitSSASExpression = true;
            else if(arg == "-module")
                parameters.moduleName = argv[++i];
            else if(arg == "-o")
                parameters.outputFileName = argv[++i];
            else if(stringBeginsWith(arg, "-o") && arg.size() > 2)
                parameters.outputFileName = arg.substr(2);
            else if(arg == "-g")
                parameters.debugInformationType = DebugInformationType::Default;
            else if(arg == "-gdwarf")
                parameters.debugInformationType = DebugInformationType::Dwarf;
            else if(arg == "-gdwarf2")
                parameters.debugInformationType = DebugInformationType::Dwarf2;
            else if(arg == "-gdwarf3")
                parameters.debugInformationType = DebugInformationType::Dwarf3;
            else if(arg == "-gdwarf4")
                parameters.debugInformationType = DebugInformationType::Dwarf4;
            else if(arg == "-gcodeview")
                parameters.debugInformationType = DebugInformationType::CodeView;
            else if(arg == "-target")
                parameters.targetName.setTriple(argv[++i]);
            else if(arg == "-mcpu")
                parameters.targetName.cpu = argv[++i];
            else if(arg == "-mfpu")
                parameters.targetName.fpu = argv[++i];
            else if(arg == "-mfloat-abi")
                parameters.targetName.floatAbi = argv[++i];
            else if(arg == "-O0")
                parameters.optimizationLevel = OptimizationLevel::O0;
            else if(arg == "-O1")
                parameters.optimizationLevel = OptimizationLevel::O1;
            else if(arg == "-O2")
                parameters.optimizationLevel = OptimizationLevel::O2;
            else if(arg == "-O3")
                parameters.optimizationLevel = OptimizationLevel::O3;
            else if(arg == "-Os")
                parameters.optimizationLevel = OptimizationLevel::Os;
            else if(arg == "-Oz")
                parameters.optimizationLevel = OptimizationLevel::Oz;
            else if(arg == "-ffast-math")
                parameters.useFastMath = true;
            else if(arg == "-fPIC")
                parameters.picMode = PICMode::PIC;
            else if(arg == "-fPIE")
                parameters.picMode = PICMode::PIE;
            else if(arg == "-semantic-analysis-only")
                parameters.semanticAnalysisOnly = true;
            else
            {
                std::cout << "Unsupported command line parameter " << arg << std::endl;
                return 1;
            }
        }
        else
        {
            parameters.inputFileNames.push_back(arg);
        }
    }

    if(parameters.inputFileNames.empty())
    {
        printHelp();
        return 0;
    }

    if(parameters.moduleName.empty())
        parameters.moduleName = basenameWithoutExtension(parameters.inputFileNames.front());

    int exitCode = 0;
    RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForTargetNameAndFeatures(parameters.targetName))->activeDuring([&]{
        auto programModule = ProgramModule::create(basename(parameters.moduleName));
        programModule->activeDuring([&]{

            for(auto &inputFileName : parameters.inputFileNames)
            {
                if(!processInputFileNamed(inputFileName))
                    exitCode = 1;
            }

            try
            {
                programModule->analyzeAllPendingProgramEntities();
            }
            catch(ExceptionWrapper &exception)
            {
                std::cerr << exception.what() << "\n";
                exitCode = 1;
            }
        });

        if(exitCode != 0 || parameters.semanticAnalysisOnly)
            return;

        if(parameters.emitSExpression)
        {
            writeStringToOutputFileNamed(sexpressionToPrettyString(programModule->asFullDefinitionSExpression()) + "\n", parameters.outputFileName);
        }
        else if(parameters.emitSSASExpression)
        {
            writeStringToOutputFileNamed(sexpressionToPrettyString(programModule->asSSAValueRequiredInPosition(ASTSourcePosition::empty())->asFullDefinitionSExpression()) + "\n", parameters.outputFileName);
        }
        else
        {
            auto backend = SSACodeGenerationBackend::makeNativeBackend();
            if(!backend)
            {
                std::cerr << "Failed to make the native code generation backend." << std::endl;
                exitCode = 1;
                return;
            }
 
            backend->setOutputMode(parameters.outputMode);
            backend->setDebugInformationType(parameters.debugInformationType);
            backend->setOutputFileName(parameters.outputFileName);
            backend->setMainInputFileName(parameters.inputFileNames.front());
            backend->setOptimizationLevel(parameters.optimizationLevel);
            backend->setUseFastMath(parameters.useFastMath);
            backend->setPICMode(parameters.picMode);
            backend->setEmitTargetIR(parameters.emitTargetIR);
            auto success = backend->processAndWriteProgramModule(programModule);
            exitCode = success ? 0 : 1;
        }
    });

    return exitCode;
}