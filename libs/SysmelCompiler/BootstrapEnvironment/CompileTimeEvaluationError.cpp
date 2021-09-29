#include "sysmel/BootstrapEnvironment/CompileTimeEvaluationError.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CompileTimeEvaluationError> CompileTimeEvaluationErrorTypeRegistration;

std::string CompileTimeEvaluationError::getDescription() const
{
    return compileTimeEvaluationErrorNode->sourcePosition->printString() + ": " + compileTimeEvaluationErrorNode->errorMessage;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius