#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

void formatUtf8Character(char c, std::ostream &out)
{
    switch(c)
    {
    case 0:
        out << "\\0";
        break;
    case '\r':
        out << "\\r";
        break;
    case '\n':
        out << "\\n";
        break;
    case '\t':
        out << "\\t";
        break;
    case '\\':
        out << "\\\\";
        break;
    default:
        out << c;
        break;
    }
}

void formatUtf32Character(char32_t c, std::ostream &out)
{
    if(c <= 127)
    {
        formatUtf8Character(char(c), out);
    }
    else
    {
        if(c <= 0xFFFF)
            out << "\\u" << std::hex << std::setfill('0') << std::setw(4) << uint32_t(c);
        else
            out << "\\U" << std::hex << std::setfill('0') << std::setw(8) << uint32_t(c);
        out << std::dec << std::setw(0);
    }
}

std::string formatStringLiteral(const std::string &value)
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

static inline bool isDigit(char c)
{
    return '0' <= c && c <= '9';
}

static inline bool isIdentifierStart(char c)
{
    return ('A' <= c && c <= 'Z') ||
        ('a' <= c && c <= 'z') ||
        '_' == c;
}

static inline bool isIdentifierMiddle(char c)
{
    return isIdentifierStart(c) || isDigit(c);
}

static bool isValidIdentifier(const std::string &value)
{
    if(value.empty() || !isIdentifierStart(value[0]))
        return false;

    for(size_t i = 1; i < value.size(); ++i)
    {
        if(!isIdentifierMiddle(value[i]))
            return false;
    }
    return true;
}

static inline bool isOperatorCharacter(int c)
{
    switch(c)
    {
    case '+':
    case '-':
    case '/':
    case '\\':
    case '*':
    case '~':
    case '<':
    case '>':
    case '=':
    case '@':
    case ',':
    case '%':
    case '|':
    case '&':
    case '?':
    case '!':
    case '^':
        return true;
    default:
        return false;
    }
}

static bool isValidBinaryOperator(const std::string &value)
{
    if(value.empty())
        return false;

    for(size_t i = 1; i < value.size(); ++i)
    {
        if(!isOperatorCharacter(value[i]))
            return false;
    }

    return true;
}

std::string formatSymbolLiteral(const std::string &value)
{
    if(isValidIdentifier(value) || isValidBinaryOperator(value))
        return "#" + value;

    std::ostringstream out;
    out << "#\"";
    for(auto c : value)
    {
        formatUtf8Character(c, out);
    }
    out << '"';
    return out.str();
}

std::string formatString(const std::string &format, const std::vector<std::string> &arguments)
{
    (void)arguments;
    return format;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius