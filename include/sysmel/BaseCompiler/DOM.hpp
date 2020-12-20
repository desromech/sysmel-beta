#ifndef SYSMEL_BASE_COMPILER_DOM_HPP
#define SYSMEL_BASE_COMPILER_DOM_HPP
#pragma once

#include "LiteralInteger.hpp"
#include "SourcePosition.hpp"
#include <variant>
#include <memory>
#include <vector>
#include <map>

namespace SysmelMoebius
{
namespace Compiler
{
namespace DOM
{

struct Array;
struct Dictionary;

struct Nil
{
    bool operator<(const Nil &) const
    {
        return false;
    }

    bool operator==(const Nil &) const
    {
        return true;
    }

    bool operator!=(const Nil &) const
    {
        return false;
    }

    friend std::ostream &operator<<(std::ostream &out, const Nil &)
    {
        return out << "nil";
    }
};

struct True
{
    bool operator<(const True &) const
    {
        return false;
    }

    bool operator==(const True &) const
    {
        return true;
    }

    bool operator!=(const True &) const
    {
        return false;
    }

    friend std::ostream &operator<<(std::ostream &out, const True &)
    {
        return out << "true";
    }
};

struct False
{
    bool operator<(const False &) const
    {
        return false;
    }

    bool operator==(const False &) const
    {
        return true;
    }

    bool operator!=(const False &) const
    {
        return false;
    }

    friend std::ostream &operator<<(std::ostream &out, const False &)
    {
        return out << "false";
    }
};

struct Void
{
    bool operator<(const Void &) const
    {
        return false;
    }

    bool operator==(const Void &) const
    {
        return true;
    }

    bool operator!=(const Void &) const
    {
        return false;
    }

    friend std::ostream &operator<<(std::ostream &out, const Void &)
    {
        return out << "void";
    }
};

struct Character
{
    uint32_t value;

    bool operator<(const Character &o) const
    {
        return value < o.value;
    }

    bool operator==(const Character &o) const
    {
        return value == o.value;
    }

    bool operator!=(const Character &o) const
    {
        return value != o.value;
    }

    friend std::ostream &operator<<(std::ostream &out, const Character &c)
    {
        return out << c.value;
    }
};

struct String
{
    std::string value;

    bool operator<(const String &o) const
    {
        return value < o.value;
    }

    bool operator==(const String &o) const
    {
        return value == o.value;
    }

    bool operator!=(const String &o) const
    {
        return value != o.value;
    }

    friend std::ostream &operator<<(std::ostream &out, const String &s)
    {
        return out << s.value;
    }
};

struct Symbol
{
    std::string value;

    bool operator<(const Symbol &o) const
    {
        return value < o.value;
    }

    bool operator==(const Symbol &o) const
    {
        return value == o.value;
    }

    bool operator!=(const Symbol &o) const
    {
        return value != o.value;
    }

    friend std::ostream &operator<<(std::ostream &out, const Symbol &s)
    {
        return out << s.value;
    }
};

typedef LiteralInteger Integer;
typedef double Float;

typedef std::variant<Void, Nil, True, False, Integer, Float, Character, String, Symbol> Key;
typedef std::variant<Void, Nil, True, False, Integer, Float, Character, String, Symbol, Array, Dictionary> Value;
inline std::ostream &operator<<(std::ostream &out, const Key &k);
inline std::ostream &operator<<(std::ostream &out, const Value &v);

struct Array
{
    typedef std::vector<Value> Elements;
    typedef Elements::iterator iterator;
    typedef Elements::const_iterator const_iterator;
    typedef Elements::value_type value_type;

    Array() {}
    Array(std::initializer_list<value_type> l) : elements(l) {}

    Elements elements;

    size_t size() const;
    bool empty() const;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void push_back(const value_type &v);

    value_type &operator[](size_t index);
    const value_type &operator[](size_t index) const;

    bool operator==(const Array &o) const;
    bool operator!=(const Array &o) const;

    friend std::ostream &operator<<(std::ostream &out, const Array &a);
};

struct Dictionary
{
    typedef std::map<Key, Value> Elements;
    typedef Elements::iterator iterator;
    typedef Elements::const_iterator const_iterator;
    typedef Elements::value_type value_type;

    Dictionary() {}
    Dictionary(std::initializer_list<value_type> l) : elements(l) {}

    Elements elements;

    size_t size() const;
    bool empty() const;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void insert(const value_type &v);

    bool operator==(const Dictionary &o) const;
    bool operator!=(const Dictionary &o) const;

    friend std::ostream &operator<<(std::ostream &out, const Dictionary &d);
};

// Array methods
inline size_t Array::size() const
{
    return elements.size();
}

inline bool Array::empty() const
{
    return elements.empty();
}

inline Array::iterator Array::begin()
{
    return elements.begin();
}

inline Array::const_iterator Array::begin() const
{
    return elements.begin();
}

inline Array::iterator Array::end()
{
    return elements.end();
}

inline Array::const_iterator Array::end() const
{
    return elements.end();
}

inline void Array::push_back(const value_type &v)
{
    elements.push_back(v);
}

inline Array::value_type &Array::operator[](size_t index)
{
    return elements[index];
}

inline const Array::value_type &Array::operator[](size_t index) const
{
    return elements[index];
}

inline bool Array::operator==(const Array &o) const
{
    return elements == o.elements;
}

inline bool Array::operator!=(const Array &o) const
{
    return elements != o.elements;
}

inline std::ostream &operator<<(std::ostream &out, const Array &a)
{
    out << '(';
    for(size_t i = 0; i < a.elements.size(); ++i)
    {
        if(i > 0)
            out << ' ';
        out << a.elements[i];
    }

    return out << ')';
}

// Dictionary methods
inline size_t Dictionary::size() const
{
    return elements.size();
}

inline bool Dictionary::empty() const
{
    return elements.empty();
}

inline Dictionary::iterator Dictionary::begin()
{
    return elements.begin();
}

inline Dictionary::const_iterator Dictionary::begin() const
{
    return elements.begin();
}

inline Dictionary::iterator Dictionary::end()
{
    return elements.end();
}

inline Dictionary::const_iterator Dictionary::end() const
{
    return elements.end();
}

inline void Dictionary::insert(const value_type &v)
{
    elements.insert(v);
}

inline bool Dictionary::operator==(const Dictionary &o) const
{
    return elements == o.elements;
}

inline bool Dictionary::operator!=(const Dictionary &o) const
{
    return elements != o.elements;
}

inline std::ostream &operator<<(std::ostream &out, const Dictionary &d)
{
    out << "#{";
    for(const auto& [key, value] : d.elements)
    {
        out << key << " : " << value << ". ";
    }

    return out << '}';
}

inline std::ostream &operator<<(std::ostream &out, const Key &key)
{
    std::visit([&](const auto &elem) { out << elem; }, key);
    return out;
}

inline std::ostream &operator<<(std::ostream &out, const Value &value)
{
    std::visit([&](const auto &elem) { out << elem; }, value);
    return out;
}

struct ParseError
{
    SourcePosition location;
    std::string errorMessage;
};

typedef std::variant<Value, ParseError> ParseResult;

ParseResult parseWithSysmelSyntax(const SourceCollectionPtr &source);

inline ParseResult parseStringWithSysmelSyntax(const std::string &text, const std::string &sourceName = "")
{
    return parseWithSysmelSyntax(std::make_shared<SourceCollection> (text, sourceName));
}

} // End of namespace DOM
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_BASE_COMPILER_DOM_LITERAL_HPP
