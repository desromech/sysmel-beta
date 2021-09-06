#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/StringUtilities.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralSymbol> literalSymbolTypeRegistration;

static std::unordered_map<std::string, std::shared_ptr<LiteralSymbol>> SymbolInternTable;

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

LiteralSymbolPtr LiteralSymbol::makeFor(const std::string &value)
{
    return intern(value);
}

bool LiteralSymbol::isLiteralSymbol() const
{
    return true;
}

std::string LiteralSymbol::printString() const
{
    std::ostringstream out;
    out << "#\"";
    for(auto c : value)
    {
        formatUtf8Character(c, out);
    }
    out << '"';
    return out.str();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius