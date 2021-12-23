#include "Frontend/SysmelSyntax/SysmelLanguageSupport.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSAValue.hpp"
#include "Environment/SSACodeGenerationBackend.hpp"

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
    SSACodeGenerationOutputMode outputMode = SSACodeGenerationOutputMode::Executable;
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

std::string basename(const std::string &name)
{
    auto pos = name.rfind('.');
    return pos != std::string::npos ? name.substr(0, pos) : name;
}

int main(int argc, const char *argv[])
{
    CompilerParameters parameters;


    for(int i = 1; i < argc; ++i)
    {
        auto arg = std::string(argv[i]);
        if(arg.size() > 1 && arg[0] == '-')
        {
            if(arg == "-c")
                parameters.outputMode = SSACodeGenerationOutputMode::ObjectFile;
            else if(arg == "-S")
                parameters.outputMode = SSACodeGenerationOutputMode::Assembly;
            else if(arg == "-shared")
                parameters.outputMode = SSACodeGenerationOutputMode::SharedLibrary;
            else if(arg == "-emit-target-ir")
                parameters.emitTargetIR = true;
            else if(arg == "-emit-sexpr")
                parameters.emitSExpression = true;
            else if(arg == "-emit-ssa-sexpr")
                parameters.emitSSASExpression = true;
            else if(arg == "-o")
                parameters.outputFileName = argv[++i];
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


    if(parameters.moduleName.empty())
    {
        parameters.moduleName = parameters.inputFileNames.front();
    }

    int exitCode = 0;
    RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForHost())->activeDuring([&]{
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

        if(exitCode != 0)
            return;

        if(parameters.emitSExpression)
            writeStringToOutputFileNamed(sexpressionToPrettyString(programModule->asFullDefinitionSExpression()) + "\n", parameters.outputFileName);
        else if(parameters.emitSSASExpression)
            writeStringToOutputFileNamed(sexpressionToPrettyString(programModule->asSSAValueRequiredInPosition(ASTSourcePosition::empty())->asFullDefinitionSExpression()) + "\n", parameters.outputFileName);
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
            backend->setOutputFileName(parameters.outputFileName);
            backend->setEmitTargetIR(parameters.emitTargetIR);
            auto success = backend->processAndWriteProgramModule(programModule);
            exitCode = success ? 0 : 1;
        }
    });

    return exitCode;
}