#ifndef SYSMEL_COMPILER_PARSER_RANGE_HPP
#define SYSMEL_COMPILER_PARSER_RANGE_HPP
#pragma once

#include <algorithm>
#include "../Environment/Assert.hpp"

namespace Sysmel
{
namespace Frontend
{

/**
 * I represent a range of elements in a collection
 */
template<typename ST, typename CP, typename BC, typename PT>
struct CollectionRange
{
    typedef CP CollectionPtr;
    typedef BC BaseCollection;
    typedef ST SelfType;
    typedef PT PeekType;

    CollectionPtr collection;
    size_t startPosition;
    size_t endPosition;

    static SelfType forCollection(const CollectionPtr &collection)
    {
        return SelfType{{collection, 0, collection->size()}};
    }

    static SelfType forEndOfCollection(const CollectionPtr &collection)
    {
        return SelfType{{collection, collection->size(), collection->size()}};
    }

    auto start() const
    {
        return collection->begin() + startPosition;
    }

    auto end() const
    {
        return collection->begin() + endPosition;
    }

    const BaseCollection content() const
    {
        return BaseCollection(start(), end());
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

    SelfType until(size_t increment) const
    {
        return SelfType {{collection, startPosition, std::min(startPosition + increment, endPosition)}};
    }

    SelfType until(const SelfType &other) const
    {
        sysmelAssert(collection == other.collection);
        return SelfType{{collection, startPosition, other.startPosition}};
    }

    SelfType untilEndOf(const SelfType &other) const
    {
        sysmelAssert(collection == other.collection);
        return SelfType{{collection, startPosition, other.endPosition}};
    }

    PeekType peek(intptr_t offset = 0) const
    {
        auto resultingPosition = size_t(startPosition + offset);
        return resultingPosition < collection->size() ? (*collection)[resultingPosition] : static_cast<const SelfType*> (this)->eofValue();
    }

    PeekType front() const
    {
        return startPosition < collection->size() ? (*collection)[startPosition] : static_cast<const SelfType*> (this)->eofValue();
    }

    PeekType back() const
    {
        return (endPosition > 0 && endPosition <= collection->size()) ?
            (*collection)[endPosition - 1]
            : static_cast<const SelfType*> (this)->eofValue();
    }

    PeekType next()
    {
        if(startPosition >= endPosition)
            return static_cast<const SelfType*> (this)->eofValue();
        return (*collection)[startPosition++];
    }

    template<typename FT>
    void skipWhile(const FT &f)
    {
        while(f(peek()))
            advance();
    }

    bool operator==(const SelfType &o) const
    {
        return collection == o.collection && startPosition == o.startPosition && endPosition == o.endPosition;
    }

    bool operator!=(const SelfType &o) const
    {
        return collection != o.collection || startPosition != o.startPosition || endPosition != o.endPosition;;
    }

    size_t hash() const
    {
        //return std::hash<CollectionPtr> ()(collection) ^ std::hash<size_t> ()(startPosition) ^ std::hash<size_t> ()(endPosition);
        const size_t HashConstant = size_t(1103515245);

        auto result = std::hash<CollectionPtr> ()(collection);
        result = result * HashConstant + std::hash<size_t> ()(startPosition);
        result = result * HashConstant + std::hash<size_t> ()(endPosition);
        return result;
    }
};

} // End of namespace Frontend
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_PARSER_RANGE_HPP
