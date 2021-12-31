#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSourcePosition> ASTSourcePositionTypeRegistration;

MethodCategories ASTSourcePosition::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding("sourceCodeFileName", &ASTSourcePosition::getSourceCodeFileName),
            makeMethodBinding("sourceText", &ASTSourcePosition::getSourceText),
            makeMethodBinding("line", &ASTSourcePosition::getLine),
            makeMethodBinding("column", &ASTSourcePosition::getColumn),
            makeMethodBinding("startLine", &ASTSourcePosition::getLine),
            makeMethodBinding("startColumn", &ASTSourcePosition::getColumn),
            makeMethodBinding("endLine", &ASTSourcePosition::getEndLine),
            makeMethodBinding("endColumn", &ASTSourcePosition::getEndColumn),
        }}
    };
}

bool ASTSourcePosition::isASTSourcePosition() const
{
    return true;
}

std::string ASTSourcePosition::getSourceCodeFileName() const
{
    return std::string();
}

int ASTSourcePosition::getLine() const
{
    return 0;
}

int ASTSourcePosition::getColumn() const
{
    return 0;
}

int ASTSourcePosition::getEndLine() const
{
    return 0;
}

int ASTSourcePosition::getEndColumn() const
{
    return 0;
}

std::string ASTSourcePosition::getSourceText() const
{
    return std::string();
}

} // End of namespace Environment
} // End of namespace Sysmel