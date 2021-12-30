#include "Environment/SemanticError.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SemanticError> SemanticErrorTypeRegistration;

std::string SemanticError::getDescription() const
{
    return semanticErrorNode->sourcePosition->printString() + ": Semantic Analysis Error: " + semanticErrorNode->errorMessage;
}

} // End of namespace Environment
} // End of namespace Sysmel