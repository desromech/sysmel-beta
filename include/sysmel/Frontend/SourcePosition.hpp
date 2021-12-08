#ifndef SYSMEL_COMPILER_SOURCE_POSITION_HPP
#define SYSMEL_COMPILER_SOURCE_POSITION_HPP
#pragma once

#include "SourceCollection.hpp"
#include "CollectionRange.hpp"
#include <ostream>
#include <functional>
#include <algorithm>

namespace Sysmel
{
namespace Frontend
{

/**
 * I represent the string data from a source collection.
 */
struct SourcePosition : CollectionRange<SourcePosition, SourceCollectionPtr, std::string, int>
{
    typedef int peek_type;

    int eofValue() const
    {
        return -1;
    }

    std::string text() const
    {
        return content();
    }

    const std::string &sourceName() const
    {
        return collection->name;
    }

    SourceLineColumn startLineColumn() const
    {
        return collection->lineAndColumnForIndex(startPosition);
    }

    SourceLineColumn endLineColumn() const
    {
        return collection->lineAndColumnForIndex(endPosition);
    }

    friend std::ostream &operator<<(std::ostream &out, SourcePosition &position)
    {
        auto lineColumn = position.startLineColumn();
        return out << position.sourceName() << ':' << lineColumn.line << ':' << lineColumn.column;
    }
};

} // End of namespace Frontend
} // End of namespace Sysmel

namespace std
{
template<>
struct hash<Sysmel::Frontend::SourcePosition>
{
    size_t operator()(const Sysmel::Frontend::SourcePosition &position) const
    {
        return position.hash();
    }
};
}

#endif //SYSMEL_COMPILER_SOURCE_POSITION_HPP
