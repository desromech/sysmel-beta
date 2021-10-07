#include "sysmel/BootstrapEnvironment/LiteralSymbol.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralSymbol> literalSymbolTypeRegistration;

static std::unordered_map<std::string, std::shared_ptr<LiteralSymbol>> SymbolInternTable;

TypePtr Type::getLiteralSymbolValue()
{
    return LiteralSymbol::__staticType__();
}

std::shared_ptr<LiteralSymbol> LiteralSymbol::intern(const std::string &value)
{
    auto it = SymbolInternTable.find(value);
    if(it != SymbolInternTable.end())
    {
        return it->second;
    }

    auto newSymbol = std::make_shared<LiteralSymbol> ();
    newSymbol->value = value;
    SymbolInternTable.insert(std::make_pair(value, newSymbol));
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

std::string LiteralSymbol::printString() const
{
    return formatSymbolLiteral(value);
}

SExpression LiteralSymbol::asSExpression() const
{
    return SExpressionSymbol{value};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius