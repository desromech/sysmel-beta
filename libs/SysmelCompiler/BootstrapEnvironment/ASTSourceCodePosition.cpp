#include "sysmel/BootstrapEnvironment/ASTSourceCodePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourceCode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSourceCodePosition> ASTSourceCodePositionTypeRegistration;

bool ASTSourceCodePosition::isASTSourceCodePosition() const
{
    return true;
}

std::string ASTSourceCodePosition::printString() const
{
    // GNU printing standard. https://www.gnu.org/prep/standards/html_node/Errors.html
    std::ostringstream out;
    out << sourceCode->name;
    out << ':' << startLine;
    out << '.' << startColumn;

    if(startLine != endLine || startColumn != endColumn)
    {
        out << '-';
        if(startLine != endLine)
        {
            out << endLine;
            if(startColumn != endColumn)
                out << '.' << endColumn;
        }
        else
        {
            out << endColumn;
        }
    }

    return out.str();
}

SExpression ASTSourceCodePosition::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"position"}},
        sourceCode->asSExpression(),
        LargeInteger{startPosition},
        LargeInteger{endPosition},

        LargeInteger{startLine},
        LargeInteger{startColumn},

        LargeInteger{endLine},
        LargeInteger{endColumn},
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius