#ifndef SYSMEL_COMPILER_PARSER_SOURCE_POSITION_HPP
#define SYSMEL_COMPILER_PARSER_SOURCE_POSITION_HPP
#pragma once

#include "SourceCollection.hpp"
#include <ostream>
#include <algorithm>

namespace SysmelMoebius
{
namespace Compiler
{

/**
 * I represent the string data from a source collection.
 */
struct SourcePosition
{
    SourceCollectionPtr sourceCollection;
    size_t startPosition;
    size_t endPosition;

    static SourcePosition forSourceCollection(const SourceCollectionPtr &sourceCollection)
    {
        return SourcePosition{sourceCollection, 0, sourceCollection->text.size()};
    }

    auto start() const
    {
        return sourceCollection->text.begin() + startPosition;
    }

    auto end() const
    {
        return sourceCollection->text.begin() + endPosition;
    }

    const std::string text() const
    {
        return std::string(start(), end());
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_t size() const
    {
        return startPosition <= endPosition ? endPosition - startPosition : 0;
    }

    void advance(size_t increment=1)
    {
        startPosition += increment;
    }

    void advanceToEnd()
    {
        startPosition = endPosition;
    }

    SourcePosition until(size_t increment) const
    {
        return SourcePosition {sourceCollection, startPosition, std::min(startPosition + increment, endPosition)};
    }

    SourcePosition until(SourcePosition &other) const
    {
        return SourcePosition {sourceCollection, startPosition, other.startPosition};
    }

    int peek(size_t offset = 0) const
    {
        return offset < size() ? sourceCollection->text[startPosition + offset] : -1;
    }

    int next()
    {
        if(startPosition >= endPosition)
            return -1;
        return sourceCollection->text[++startPosition];
    }

    template<typename FT>
    void skipWhile(const FT &f)
    {
        while(f(peek()))
            advance();
    }
    
    const std::string &sourceName() const
    {
        return sourceCollection->name;
    }

    friend std::ostream &operator<<(std::ostream &out, SourcePosition &position)
    {
        out << position.sourceCollection->name << "[" << position.startPosition << ":" << position.endPosition;
        return out;
    }
};

} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_SOURCE_POSITION_HPP
