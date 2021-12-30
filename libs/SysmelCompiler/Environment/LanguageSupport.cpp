#include "Environment/LanguageSupport.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/ASTParseErrorValidator.hpp"
#include "Environment/CleanUpScope.hpp"
#include "Environment/LexicalScope.hpp"
#include "Environment/ProgramEntityScope.hpp"
#include "Environment/CompiledMethod.hpp"
#include "Environment/Module.hpp"
#include "Environment/Namespace.hpp"
#include "Environment/Type.hpp"
#include "Environment/StringUtilities.hpp"
#include <fstream>
#include <iostream>

namespace Sysmel
{
namespace Environment
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
    auto result = basicMakeObject<ASTAnalysisEnvironment> ();
    result->languageSupport = selfFromThis();
    result->programEntityForPublicDefinitions = Module::getActive()->getGlobalNamespace();
    result->lexicalScope = LexicalScope::makeWithParent(ProgramEntityScope::make(createDefaultTopLevelLexicalScope(), result->programEntityForPublicDefinitions));
    result->defaultArgumentType = Type::getAnyValueType();
    result->defaultTemplateArgumentType = Type::getAnyValueType();
    result->defaultResultType = Type::getAnyValueType();
    result->defaultVariableType = Type::getAnyValueType();
    result->literalValueInferrenceType = Type::getAnyValueType();
    return result;
}

LexicalScopePtr LanguageSupport::createDefaultTopLevelLexicalScope()
{
    return basicMakeObject<LexicalScope> ();
}

ASTAnalysisEnvironmentPtr LanguageSupport::createMakeLiteralArrayAnalysisEnvironment()
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> ();
    result->lexicalScope = createMakeLiteralArrayTopLevelLexicalScope();
    result->languageSupport = selfFromThis();
    result->isLiteralArrayAnalysisEnvironment = true;
    return result;
}

LexicalScopePtr LanguageSupport::createMakeLiteralArrayTopLevelLexicalScope()
{
    return basicMakeObject<LexicalScope> ();
}

ASTNodePtr LanguageSupport::parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName)
{
    (void)sourceString;
    (void)sourceStringName;
    SysmelSelfSubclassResponsibility();
}

ASTNodePtr LanguageSupport::parseFileNamed(const std::string &fileName)
{
    auto absoluteFileName = makeAbsolutePath(fileName);
    return parseSourceStringNamed(readContentFromFileNamed(absoluteFileName), absoluteFileName);
}

CompiledMethodPtr LanguageSupport::analyzeASTInEnvironment(const ASTNodePtr &ast, const ASTAnalysisEnvironmentPtr &environment)
{
    // Validate the AST.
    {
        auto parseErrorValidator = basicMakeObject<ASTParseErrorValidator> ();
        parseErrorValidator->visitNode(ast);
        auto parseError = parseErrorValidator->makeCompilationError();
        if(parseError)
            parseError->signal();
    }

    auto scriptMethod = basicMakeObject<CompiledMethod> ();
    scriptMethod->setFunctionSignature(Type::getAutoType(), TypePtrList{});
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

} // End of namespace Environment
} // End of namespace Sysmel