#include "Environment/StringUtilities.hpp"
#include <iomanip>
#include <sstream>

#ifndef _WIN32
#include <limits.h>
#include <unistd.h>

#else
#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN 
#endif

#include <windows.h>
#endif

namespace Sysmel
{
namespace Environment
{

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

bool isValidIdentifierString(const std::string &string)
{
    if(string.empty() || !isIdentifierStart(string[0]))
        return false;
    
    for(auto c : string)
    {
        if(!isIdentifierMiddle(c))
            return false;
    }
    return true;
}

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

std::string basename(const std::string &path)
{
    auto pos = std::string::npos;

    for(size_t i = 0; i < path.size(); ++i)
    {
        auto c = path[i];
        if(c == '/' || c == '\\')
            pos = i + 1;
    }

    return pos != std::string::npos ? path.substr(pos) : path;
}

std::string basenameWithoutExtension(const std::string &path)
{
    auto bn = basename(path);
    auto pos = bn.rfind('.');
    return pos != std::string::npos ? bn.substr(0, pos) : bn;
}

std::string dirname(const std::string &path)
{
    auto pos = std::string::npos;

    for(size_t i = 0; i < path.size(); ++i)
    {
        auto c = path[i];
        if(c == '/' || c == '\\')
            pos = i;
    }

    return pos != std::string::npos ? path.substr(0, pos) : std::string();
}


#ifdef _WIN32
std::wstring toUTF16(const std::string &s)
{
    auto requiredBufferSize = MultiByteToWideChar(CP_UTF8, 0, s.data(), int(s.size()), nullptr, 0);
    if(requiredBufferSize == 0)
        return std::wstring();

    std::wstring result;
    result.resize(requiredBufferSize);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), int(s.size()), result.data(), requiredBufferSize);
    return result;
}

std::string toUTF8(const std::wstring &s)
{
    auto requiredBufferSize = WideCharToMultiByte(CP_UTF8, 0, s.data(), int(s.size()), nullptr, 0, nullptr, nullptr);
    if(requiredBufferSize == 0)
        return std::string();

    std::string result;
    result.resize(requiredBufferSize);
    WideCharToMultiByte(CP_UTF8, 0, s.data(), int(s.size()), result.data(), requiredBufferSize, nullptr, nullptr);
    return result;
}

#endif

std::string makeAbsolutePath(const std::string &path)
{
    if(path == "-")
        return path;

#ifdef _WIN32
    std::vector<wchar_t> buffer;
    buffer.resize(32767);

    auto resultSize = GetFullPathNameW(toUTF16(path).c_str(), DWORD(buffer.size()), buffer.data(), NULL);
    return toUTF8(std::wstring(buffer.data(), buffer.data() + resultSize));
#else
    std::unique_ptr<char[]> buffer(new char[PATH_MAX+1]);
    auto result = realpath(path.c_str(), buffer.get());
    return result ? result : path;
#endif

    return path;
}

std::vector<std::string> split(const std::string &string, char delim)
{
    std::vector<std::string> result;
    std::string currentString;
    for(auto c : string)
    {
        if(c == delim)
        {
            result.push_back(currentString);
            currentString.clear();
        }
        else
        {
            currentString.push_back(c);
        }
    }

    return result;
}

bool stringBeginsWith(const std::string &string, const std::string &prefix)
{
    if(string.size() < prefix.size())
        return false;

    for(size_t i = 0; i < prefix.size(); ++i)
    {
        if(string[i] != prefix[i])
            return false;
    }

    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel