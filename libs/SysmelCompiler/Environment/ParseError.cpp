#include "Environment/ParseError.hpp"
#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ParseError> ParseErrorTypeRegistration;

std::string ParseError::getDescription() const
{
    return parseErrorNode->sourcePosition->printString() + ": Parse Error: " + parseErrorNode->errorMessage;
}

} // End of namespace Environment
} // End of namespace Sysmel