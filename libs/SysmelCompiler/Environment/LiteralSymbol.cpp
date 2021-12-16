#include "Environment/LiteralSymbol.hpp"
#include "Environment/Type.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralSymbol> literalSymbolTypeRegistration;

static std::unordered_map<std::string, ObjectPtr<LiteralSymbol>> SymbolInternTable;

TypePtr Type::getLiteralSymbolValue()
{
    return LiteralSymbol::__staticType__();
}

ObjectPtr<LiteralSymbol> LiteralSymbol::intern(const std::string &value)
{
    auto &table = RuntimeContext::getActive()->symbolInternTable;
    auto it = table.find(value);
    if(it != table.end())
        return it->second;

    auto newSymbol = basicMakeObject<LiteralSymbol> ();
    newSymbol->value = value;
    table.insert({value, newSymbol});
    return newSymbol;
}

AnyValuePtr internSymbol(const std::string &symbolValue)
{
    return LiteralSymbol::intern(symbolValue);
}

LiteralSymbolPtr LiteralSymbol::makeFor(const std::string &value)
{
    return intern(value);
}

bool LiteralSymbol::isLiteralSymbol() const
{
    return true;
}

bool LiteralSymbol::isLiteralIdentifierSymbol() const
{
    return isValidIdentifierString(value);
}

bool LiteralSymbol::isAnonymousNameSymbol() const
{
    return value.empty() || value == "_";
}

AnyValuePtr LiteralSymbol::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitLiteralSymbol(selfFromThis());
}

std::string LiteralSymbol::printString() const
{
    return formatSymbolLiteral(value);
}

SExpression LiteralSymbol::asSExpression() const
{
    return SExpressionSymbol{value};
}

AnyValuePtr LiteralSymbol::asUnarySelectorConvertedToIdentifier() const
{
    if(value.size() >= 2 && value.back() == ':')
    {
        auto withoutColon = value.substr(0, value.size() - 1);
        if(isValidIdentifierString(withoutColon))
            return internSymbol(withoutColon);
    }
    
    return SuperType::asUnarySelectorConvertedToIdentifier();
}

} // End of namespace Environment
} // End of namespace Sysmel