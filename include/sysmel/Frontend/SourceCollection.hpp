#ifndef SYSMEL_COMPILER_SOURCE_COLLECTION_HPP
#define SYSMEL_COMPILER_SOURCE_COLLECTION_HPP
#pragma once

#include <string>
#include <memory>
#include <algorithm>
#include <vector>

namespace Sysmel
{
namespace Frontend
{

struct SourceLineColumn
{
    size_t line;
    size_t column;
};

/**
 * I represent the string data from a source collection.
 */
class SourceCollection
{
public:
    typedef std::string::value_type value_type;
    typedef std::string::iterator iterator;
    typedef std::string::const_iterator const_iterator;

    SourceCollection() {}
    SourceCollection(const std::string &ctext, const std::string &cname = "")
        : text(ctext), name(cname)
    {
        buildLineStartIndex();
    }

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

    SourceLineColumn lineAndColumnForIndex(size_t index)
    {
        auto iterator = std::upper_bound(lineStartIndex.begin(), lineStartIndex.end(), index);
        --iterator;

        return SourceLineColumn{1u + size_t(iterator - lineStartIndex.begin()), 1 + index - *iterator};
    }

private:

    void buildLineStartIndex()
    {
        lineStartIndex.push_back(0);
        for(size_t i = 0; i < text.size(); ++i)
        {
            auto c = text[i];
            if(c == '\r')
            {
                if(i + 1 < text.size() && text[i+1] == '\n')
                    ++i;
                lineStartIndex.push_back(i + 1);
            }
            else if(c == '\n')
            {
                lineStartIndex.push_back(i + 1);
            }
        }

    }

    std::vector<size_t> lineStartIndex;
};

typedef std::shared_ptr<SourceCollection> SourceCollectionPtr;

} // End of namespace Frontend
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_SOURCE_COLLECTION_HPP
