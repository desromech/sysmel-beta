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

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyWithCleanUpcope(const CleanUpScopePtr &newCleanUpScope)
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> (*this);
    result->cleanUpScope = newCleanUpScope;
    return result;
}

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyForPublicProgramEntityBody(const ProgramEntityPtr &publicProgramEntity, const ASTSourcePositionPtr &sourcePosition)
{
    auto result = basicMakeObject<ASTAnalysisEnvironment> (*this);
    result->programEntityForPublicDefinitions = publicProgramEntity;
    result->cleanUpScope = CleanUpScope::makeWithParent(cleanUpScope);
    result->lexicalScope = LexicalScope::makeWithParent(ProgramEntityScope::make(lexicalScope, publicProgramEntity), sourcePosition);
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

bool ASTAnalysisEnvironment::hasValidLiteralValueInferrenceType() const
{
    return literalValueInferrenceType && !literalValueInferrenceType->isVoidType();
}

} // End of namespace Environment
} // End of namespace Sysmel