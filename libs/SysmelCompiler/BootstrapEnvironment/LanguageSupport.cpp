#include "sysmel/BootstrapEnvironment/LanguageSupport.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorValidator.hpp"
#include "sysmel/BootstrapEnvironment/CleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"
#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include <fstream>
#include <iostream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LanguageSupport> LanguageSupportTypeRegistration;

static std::string readContentFromFileNamed(const std::string &fileName)
{
    if(fileName == "-")
    {
        return std::string(std::istreambuf_iterator<char> (std::cin), std::istreambuf_iterator<char> ());
    }

    std::ifstream in(fileName);
    if(!in.good())
    {
        signalNewWithMessage<Error> ("Failed to read file named: " + fileName);
    }

    return std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());
}

MethodCategories LanguageSupport::__instanceMethods__()
{
    return MethodCategories{
        {"parsing", {
            makeMethodBinding("parseSourceString:named:", &LanguageSupport::parseSourceStringNamed),
            makeMethodBinding("parseFileNamed:", &LanguageSupport::parseFileNamed),
        }},
    };
}

ASTAnalysisEnvironmentPtr LanguageSupport::createDefaultAnalysisEnvironment()
{
    auto result = std::make_shared<ASTAnalysisEnvironment> ();
    result->lexicalScope = createDefaultTopLevelLexicalScope();
    result->cleanUpScope = CleanUpScope::makeEmpty();
    result->languageSupport = shared_from_this();
    return result;
}

LexicalScopePtr LanguageSupport::createDefaultTopLevelLexicalScope()
{
    return std::make_shared<LexicalScope> ();
}

ASTAnalysisEnvironmentPtr LanguageSupport::createMakeLiteralArrayAnalysisEnvironment()
{
    auto result = std::make_shared<ASTAnalysisEnvironment> ();
    result->lexicalScope = createMakeLiteralArrayTopLevelLexicalScope();
    result->cleanUpScope = CleanUpScope::makeEmpty();
    result->languageSupport = shared_from_this();
    return result;
}

LexicalScopePtr LanguageSupport::createMakeLiteralArrayTopLevelLexicalScope()
{
    return std::make_shared<LexicalScope> ();
}

ASTNodePtr LanguageSupport::parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName)
{
    (void)sourceString;
    (void)sourceStringName;
    SysmelSelfSubclassResponsibility();
}

ASTNodePtr LanguageSupport::parseFileNamed(const std::string &fileName)
{
    return parseSourceStringNamed(readContentFromFileNamed(fileName), fileName);
}

CompiledMethodPtr LanguageSupport::analyzeASTInEnvironment(const ASTNodePtr &ast, const ASTAnalysisEnvironmentPtr &environment)
{
    // Validate the AST.
    {
        auto parseErrorValidator = std::make_shared<ASTParseErrorValidator> ();
        parseErrorValidator->visitNode(ast);
        auto parseError = parseErrorValidator->makeCompilationError();
        if(parseError)
            parseError->signal();
    }

    auto scriptMethod = std::make_shared<CompiledMethod> ();
    scriptMethod->setDefinition(ast, ast, environment);
    scriptMethod->ensureSemanticAnalysis();
    return scriptMethod;
}

CompiledMethodPtr LanguageSupport::semanticAnalyzeStringNamed(const std::string &sourceString, const std::string &sourceStringName)
{
    return analyzeASTInEnvironment(parseSourceStringNamed(sourceString, sourceStringName), createDefaultAnalysisEnvironment());
}

CompiledMethodPtr LanguageSupport::semanticAnalyzeFileNamed(const std::string &fileName)
{
    return analyzeASTInEnvironment(parseFileNamed(fileName), createDefaultAnalysisEnvironment());
}

AnyValuePtr LanguageSupport::evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName)
{
    return semanticAnalyzeStringNamed(sourceString, sourceStringName)->runWithArgumentsIn(internSymbol("script"), AnyValuePtrList{}, getNilConstant());
}

AnyValuePtr LanguageSupport::evaluateFileNamed(const std::string &fileName)
{
    return semanticAnalyzeFileNamed(fileName)->runWithArgumentsIn(internSymbol("script"), AnyValuePtrList{}, getNilConstant());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius