#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

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