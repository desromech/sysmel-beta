#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include <iomanip>
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

    for(auto c : value)
    {
        if(!isOperatorCharacter(c))
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
    std::string result;
    auto reservedSize = format.size();
    for(const auto &arg : arguments)
        reservedSize += arg.size();
    result.reserve(reservedSize);

    size_t currentParentCount = 0;
    size_t openParentCount = 0;
    size_t nextArgumentIndex = 0;
    for(auto c : format)
    {
        if(openParentCount > 0)
        {
            if(c == '{')
            {
                ++currentParentCount;
                ++openParentCount;
                result.push_back(c);
            }
            else if(c == '}')
            {
                --openParentCount;
                if(openParentCount > 0)
                    result.push_back(c);
                else
                {
                    if(currentParentCount == 1 && nextArgumentIndex < arguments.size())
                        result += arguments[nextArgumentIndex];
                }
            }
            else if(openParentCount > 1)
            {
                // Pass through escaped characters.
                result.push_back(c);
            }
            else if(currentParentCount == 1 && isDigit(c))
            {
                // Parse the argument index digit.
                nextArgumentIndex = nextArgumentIndex * 10 + c - '0';
            }
        }
        else
        {
            if(c == '{')
            {
                ++openParentCount;
                currentParentCount = 1;
                nextArgumentIndex = 0;
            }
            else
            {
                result.push_back(c);
            }
        }
    }

    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius