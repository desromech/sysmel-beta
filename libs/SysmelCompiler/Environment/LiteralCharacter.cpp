#include "Environment/LiteralCharacter.hpp"
#include "Environment/StringUtilities.hpp"

#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/PrimitiveCharacterType.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <algorithm>
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LiteralCharacter> literalCharacterTypeRegistration;

TypePtr WrapperTypeFor<char32_t>::apply()
{
    return LiteralCharacter::__staticType__();
}

bool LiteralCharacter::isLiteralCharacter() const
{
    return true;
}

AnyValuePtr LiteralCharacter::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitLiteralCharacter(selfFromThis());
}

MethodCategories LiteralCharacter::__instanceMethods__()
{
    return MethodCategories{ };
}

TypePtr LiteralCharacter::__asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    if(concreteLiterals && node->isASTLiteralValueNode() && !environment->hasValidLiteralValueInferrenceType())
        return Char32::__staticType__();

    return SuperType::__asInferredTypeForWithModeInEnvironment__(selfType, node, mode, isMutable, concreteLiterals, environment);

}
std::string LiteralCharacter::asString() const
{
    std::string result;
    result.reserve(1);
    result[0] = char(value.wordAt(0));
    // FIXME: Encode the Char32 as UTF8!!!
    return result;
}

std::string LiteralCharacter::printString() const
{
    std::ostringstream out;
    out << '\'';
    formatUtf32Character(value.wordAt(0), out);
    out << '\'';
    return out.str();
}

SExpression LiteralCharacter::asSExpression() const
{
    return unwrapAsChar32();
}

} // End of namespace Environment
} // End of namespace Sysmel