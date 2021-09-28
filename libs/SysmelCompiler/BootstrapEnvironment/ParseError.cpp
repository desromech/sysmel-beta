#include "sysmel/BootstrapEnvironment/ParseError.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ParseError> ParseErrorTypeRegistration;

std::string ParseError::getDescription() const
{
    return parseErrorNode->sourcePosition->printString() + ": " + parseErrorNode->errorMessage;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius