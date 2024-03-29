#include "Environment/ASTSourceCodePosition.hpp"
#include "Environment/ASTSourceCode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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

std::string ASTSourceCodePosition::getSourceCodeFileName() const
{
    return sourceCode->name;
}

std::string ASTSourceCodePosition::getSourceText() const
{
    if(startPosition > endPosition)
        return std::string();

    return sourceCode->contents.substr(startPosition, endPosition - startPosition);
}

int ASTSourceCodePosition::getLine() const
{
    return int(startLine);
}

int ASTSourceCodePosition::getColumn() const
{
    return int(startColumn);
}

int ASTSourceCodePosition::getEndLine() const
{
    return int(endLine);
}
int ASTSourceCodePosition::getEndColumn() const
{
    return int(endColumn);
}

SExpression ASTSourceCodePosition::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"position"}},
        sourceCode->asSExpression(),
        LargeInteger{uint64_t(startPosition)},
        LargeInteger{uint64_t(endPosition)},

        LargeInteger{uint64_t(startLine)},
        LargeInteger{uint64_t(startColumn)},

        LargeInteger{uint64_t(endLine)},
        LargeInteger{uint64_t(endColumn)},
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel