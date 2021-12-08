#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/LexicalScope.hpp"
#include "Environment/ProgramEntityScope.hpp"
#include "Environment/CleanUpScope.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTAnalysisEnvironment> ASTAnalysisEnvironmentTypeRegistration;

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyWithLexicalScope(const LexicalScopePtr &newScope)
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> (*this);
    result->lexicalScope = newScope;
    return result;
}

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyWithCleanUpcope(const CleanUpScopePtr &cleanUpScope)
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> (*this);
    result->cleanUpScope = cleanUpScope;
    return result;
}

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyForPublicProgramEntityBody(const ProgramEntityPtr &publicProgramEntity)
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> (*this);
    result->programEntityForPublicDefinitions = publicProgramEntity;
    result->cleanUpScope = CleanUpScope::makeWithParent(cleanUpScope);
    result->lexicalScope = LexicalScope::makeWithParent(ProgramEntityScope::make(lexicalScope, publicProgramEntity));
    result->continueLevelCount = 0;
    result->breakLevelCount = 0;
    return result;
}

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyWithBreakAndContinueLevel(uint32_t newBreakLevelCount, uint32_t newContinueLevelCount)
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> (*this);
    result->breakLevelCount = newBreakLevelCount;
    result->continueLevelCount = newContinueLevelCount;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel