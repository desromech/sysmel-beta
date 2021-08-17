#include "sysmel/ObjectModel/LiteralString.hpp"
#include "sysmel/ObjectModel/StringUtilities.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{

bool LiteralString::isLiteralString() const
{
    return true;
}

std::string LiteralString::asString() const
{
    return value;
}

std::string LiteralString::printString() const
{
    std::ostringstream out;
    out << '"';
    for(auto c : value)
    {
        formatUtf8Character(c, out);
    }
    out << '"';
    return out.str();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius