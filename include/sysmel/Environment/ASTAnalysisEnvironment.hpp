#ifndef SYSMEL_ENVIRONMENT_AST_ANALYSIS_ENVIRONMENT_HPP
#define SYSMEL_ENVIRONMENT_AST_ANALYSIS_ENVIRONMENT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CleanUpScope)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LanguageSupport);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SpecificMethod);

/**
 * I specify an AST analysis environment.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTAnalysisEnvironment : public SubtypeOf<CompilerObject, ASTAnalysisEnvironment>
{
public:
    static constexpr char const __typeName__[] = "ASTAnalysisEnvironment";

    ASTAnalysisEnvironmentPtr copyWithLexicalScope(const LexicalScopePtr &newScope);
    ASTAnalysisEnvironmentPtr copyWithCleanUpcope(const CleanUpScopePtr &cleanUpScope);
    ASTAnalysisEnvironmentPtr copyForPublicProgramEntityBody(const ProgramEntityPtr &publicProgramEntity);
    ASTAnalysisEnvironmentPtr copyWithBreakAndContinueLevel(uint32_t newBreakLevelCount, uint32_t newContinueLevelCount);

    LexicalScopePtr lexicalScope;
    CleanUpScopePtr cleanUpScope;
    LanguageSupportPtr languageSupport;

    ProgramEntityPtr programEntityForPublicDefinitions;
    SpecificMethodPtr returnTargetMethod;
    ProgramEntityPtr localDefinitionsOwner;

    TypePtr defaultArgumentType;
    TypePtr defaultResultType;
    TypePtr defaultVariableType;
    TypePtr defaultTemplateArgumentType;
    TypePtr literalValueInferrenceType;

    bool isLiteralArrayAnalysisEnvironment = false;

    uint32_t continueLevelCount = 0;
    uint32_t breakLevelCount = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_ANALYSIS_ENVIRONMENT_HPP