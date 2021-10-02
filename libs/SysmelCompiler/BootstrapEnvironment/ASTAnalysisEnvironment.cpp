#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTAnalysisEnvironment> ASTAnalysisEnvironmentTypeRegistration;

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyWithLexicalScope(const LexicalScopePtr &newScope)
{
    auto result = std::make_shared<ASTAnalysisEnvironment> (*this);
    result->lexicalScope = newScope;
    return result;
}

ASTAnalysisEnvironmentPtr ASTAnalysisEnvironment::copyWithCleanUpcope(const CleanUpScopePtr &cleanUpScope)
{
    auto result = std::make_shared<ASTAnalysisEnvironment> (*this);
    result->cleanUpScope = cleanUpScope;
    return result;
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius