#include "Environment/PatternMatchingValueName.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<PatternMatchingValueName> PatternMatchingValueNameTypeRegistration;

bool PatternMatchingValueName::isHiddenNameSymbol() const
{
    return true;
}

std::string PatternMatchingValueName::printString() const
{
    return "PatternMatchingValueName at " + sourcePosition->printString();
}

SExpression PatternMatchingValueName::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"patternMatchingValueName"}},
        sourcePosition->asSExpression(),
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel