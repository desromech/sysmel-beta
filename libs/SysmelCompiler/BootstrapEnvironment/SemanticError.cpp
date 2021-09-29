#include "sysmel/BootstrapEnvironment/SemanticError.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SemanticError> SemanticErrorTypeRegistration;

std::string SemanticError::getDescription() const
{
    return semanticErrorNode->sourcePosition->printString() + ": " + semanticErrorNode->errorMessage;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius