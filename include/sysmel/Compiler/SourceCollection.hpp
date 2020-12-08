#ifndef SYSMEL_COMPILER_SOURCE_COLLECTION_HPP
#define SYSMEL_COMPILER_SOURCE_COLLECTION_HPP
#pragma once

#include <string>
#include <memory>

namespace SysmelMoebius
{
namespace Compiler
{

/**
 * I represent the string data from a source collection.
 */
struct SourceCollection
{
    typedef std::string::value_type value_type;
    typedef std::string::iterator iterator;
    typedef std::string::const_iterator const_iterator;

    SourceCollection() {}
    SourceCollection(const std::string &ctext, const std::string &cname = "")
        : text(ctext), name(cname) {}

    std::string text;
    std::string name;

    iterator begin()
    {
        return text.begin();
    }

    const_iterator begin() const
    {
        return text.begin();
    }

    iterator end()
    {
        return text.end();
    }

    const_iterator end() const
    {
        return text.end();
    }

    size_t size() const
    {
        return text.size();
    }

    value_type &operator[](size_t index)
    {
        return text[index];
    }

    const value_type &operator[](size_t index) const
    {
        return text[index];
    }
};

typedef std::shared_ptr<SourceCollection> SourceCollectionPtr;

} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_SOURCE_COLLECTION_HPP
