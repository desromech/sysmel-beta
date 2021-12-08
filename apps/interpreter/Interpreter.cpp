#include "Frontend/SysmelSyntax/SysmelLanguageSupport.hpp"
#include "Environment/AnyValue.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/SSAValue.hpp"
#include "Environment/Wrappers.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapModule.hpp"
#include "Environment/ScriptModule.hpp"
#include "Environment/CompiledMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Exception.hpp"
#include "Environment/FunctionalType.hpp"
#include <fstream>
#include <iostream>

using namespace Sysmel::Environment;

void parseString(const std::string &sourceString, const std::string &sourceName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto ast = language->parseSourceStringNamed(sourceString, sourceName);
        std::cout << ast->printString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void semanticAnalyzeString(const std::string &sourceString, const std::string &sourceName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto analyzed = language->semanticAnalyzeStringNamed(sourceString, sourceName);
        std::cout << analyzed->fullPrintString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void ssaCompileString(const std::string &sourceString, const std::string &sourceName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto analyzed = language->semanticAnalyzeStringNamed(sourceString, sourceName);
        std::cout << analyzed->asSSAValueRequiredInPosition(ASTSourcePosition::empty())->fullPrintString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void evalString(const std::string &sourceString, const std::string &sourceName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto result = language->evaluateSourceStringNamed(sourceString, sourceName);
        std::cout << result->printString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void semanticAnalyzeEvalString(const std::string &sourceString, const std::string &sourceName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto result = language->evaluateSourceStringNamed(sourceString, sourceName);
        if(result->isFunctionalTypeValue())
            result = result.staticAs<FunctionalTypeValue> ()->functionalImplementation;
        
        ScriptModule::getActive()->analyzeAllPendingProgramEntities();
        std::cout << result->fullPrintString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void ssaCompileEvalString(const std::string &sourceString, const std::string &sourceName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto result = language->evaluateSourceStringNamed(sourceString, sourceName);
        std::cout << result->asSSAValueRequiredInPosition(ASTSourcePosition::empty())->fullPrintString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void evalFileNamed(const std::string &fileName)
{
    auto language = Sysmel::Environment::SysmelLanguageSupport::uniqueInstance();
    try
    {
        auto result = language->evaluateFileNamed(fileName);
        std::cout << result->printString() << std::endl;
    }
    catch(ExceptionWrapper &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

void dumpEnvironment()
{
    auto bootstrapModule = RuntimeContext::getActive()->getBootstrapModule();
    AnyValue::__staticType__()->withAllSubtypesDo([&](const TypePtr &type) {
        std::cout << type->asString() << std::endl;
        auto superType = type->getSupertype();
        if(superType)
            std::cout << "    supertype: " << superType->asString() << std::endl;
    });
}

int main(int argc, const char *argv[])
{
    int exitCode = 0;

    RuntimeContext::createForScripting()->activeDuring([&](){
        ScriptModule::create()->activeDuring([&](){
            for(int i = 1; i < argc; ++i)
            {
                std::string arg = argv[i];
                if(arg == "-eval")
                {
                    evalString(argv[++i], "<cli>");
                }
                else if(arg == "-parse-string")
                {
                    parseString(argv[++i], "<cli>");
                }
                else if(arg == "-semantic-analyze-string")
                {
                    semanticAnalyzeString(argv[++i], "<cli>");
                }

                else if(arg == "-semantic-analyze-string-value")
                {
                    semanticAnalyzeEvalString(argv[++i], "<cli>");
                }
                else if(arg == "-ssa-for-string")
                {
                    ssaCompileString(argv[++i], "<cli>");
                }
                else if(arg == "-ssa-for-string-value")
                {
                    ssaCompileEvalString(argv[++i], "<cli>");
                }
                else if(arg == "-dump-bootstrap-env")
                {
                    dumpEnvironment();
                    return;
                }
                else if(!arg.empty() && arg[0] != '-')
                {
                    evalFileNamed(argv[i]);
                }
                else
                {
                    std::cerr << "Unsupported command line argument " << arg << std::endl;
                    exitCode = 1;
                    return;
                }
            }

            try
            {
                Module::getActive()->analyzeAllPendingProgramEntities();
            }
            catch(ExceptionWrapper &exception)
            {
                std::cerr << exception.what() << std::endl;
                exitCode = 1;
            }
       });
    });

    return exitCode;
}
