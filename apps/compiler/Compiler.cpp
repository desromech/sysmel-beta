#include "Frontend/SysmelSyntax/SysmelLanguageSupport.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSAValue.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace Sysmel::Environment;

enum class OutputMode : uint8_t
{
    Executable = 0,
    SharedLibrary,
    ObjectFile,
    Assembly,
};

struct CompilerParameters
{
    bool emitLLVM = false;
    bool emitSExpression = false;
    bool emitSSASExpression = false;
    OutputMode outputMode = OutputMode::Executable;
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

int main(int argc, const char *argv[])
{
    CompilerParameters parameters;


    for(int i = 1; i < argc; ++i)
    {
        auto arg = std::string(argv[i]);
        if(arg.size() > 1 && arg[0] == '-')
        {
            if(arg == "-c")
                parameters.outputMode = OutputMode::ObjectFile;
            else if(arg == "-S")
                parameters.outputMode = OutputMode::Assembly;
            else if(arg == "-shared")
                parameters.outputMode = OutputMode::SharedLibrary;
            else if(arg == "-emit-llvm")
                parameters.emitLLVM = true;
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

    int exitCode = 0;
    RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForHost())->activeDuring([&]{
        auto programModule = ProgramModule::create(parameters.outputFileName);
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
            std::cerr << "TODO: implement the support for generating backend specific output." << std::endl;
        }
    });

    return exitCode;
}