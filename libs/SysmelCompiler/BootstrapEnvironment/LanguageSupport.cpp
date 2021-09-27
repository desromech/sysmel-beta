#include "sysmel/BootstrapEnvironment/LanguageSupport.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/IdentifierLookupScope.hpp"
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
        throw Error("Failed to read file named: " + fileName);
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

ASTAnalysisEnvironmentPtr LanguageSupport::createDefaultAnalysisEnvironment() const
{
    auto result = std::make_shared<ASTAnalysisEnvironment> ();
    result->identifierLookupScope = std::make_shared<IdentifierLookupScope> ();
    return result;
}

ASTNodePtr LanguageSupport::parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const
{
    (void)sourceString;
    (void)sourceStringName;
    throw SubclassResponsibility();
}

ASTNodePtr LanguageSupport::parseFileNamed(const std::string &fileName) const
{
    return parseSourceStringNamed(readContentFromFileNamed(fileName), fileName);
}

AnyValuePtr LanguageSupport::evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const
{
    auto ast = parseSourceStringNamed(sourceString, sourceStringName);
    auto compiledMethod = analyzeASTInEnvironment(ast, createDefaultAnalysisEnvironment());
    return compiledMethod->runWithArgumentsIn(internSymbol("script"), AnyValuePtrList{}, getNilConstant());
}

CompiledMethodPtr LanguageSupport::analyzeASTInEnvironment(const ASTNodePtr &ast, const ASTAnalysisEnvironmentPtr &environment) const
{
    auto scriptMethod = std::make_shared<CompiledMethod> ();
    scriptMethod->setDefinition(ast, ast, environment);
    scriptMethod->ensureSemanticAnalysis();
    return scriptMethod;
}

AnyValuePtr LanguageSupport::evaluateFileNamed(const std::string &fileName) const
{
    auto ast = parseFileNamed(fileName);
    auto compiledMethod = analyzeASTInEnvironment(ast, createDefaultAnalysisEnvironment());
    return compiledMethod->runWithArgumentsIn(internSymbol("script"), AnyValuePtrList{}, getNilConstant());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius