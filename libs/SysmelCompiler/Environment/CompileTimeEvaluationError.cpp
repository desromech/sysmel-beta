#include "Environment/CompileTimeEvaluationError.hpp"
#include "Environment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompileTimeEvaluationError> CompileTimeEvaluationErrorTypeRegistration;

std::string CompileTimeEvaluationError::getDescription() const
{
    return formatString("{0}: Error caught during compile time evaluation. {1}", {{
        compileTimeEvaluationErrorNode->sourcePosition->printString(),
        compileTimeEvaluationErrorNode->errorMessage
    }});
}

} // End of namespace Environment
} // End of namespace Sysmel