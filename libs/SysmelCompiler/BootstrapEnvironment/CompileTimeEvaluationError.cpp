#include "sysmel/BootstrapEnvironment/CompileTimeEvaluationError.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CompileTimeEvaluationError> CompileTimeEvaluationErrorTypeRegistration;

std::string CompileTimeEvaluationError::getDescription() const
{
    return formatString("{0}: Error caught during compile time evaluation. {1}", {{
        compileTimeEvaluationErrorNode->sourcePosition->printString(),
        compileTimeEvaluationErrorNode->errorMessage
    }});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius