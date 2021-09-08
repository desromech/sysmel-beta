#ifndef SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
#pragma once

#include "ProgramEntity.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class Type : public SubtypeOf<ProgramEntity, Type>
{
public:
    static constexpr char const __typeName__[] = "Type";

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual TypePtr getSuperType();

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr lookupSelector(const AnyValuePtr &selector);

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);

    /// This method add a new method into the method dictionary with the specified selector.
    virtual void addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method);

    /// This method computes the rank required for matching the specified type without implicit casting.
    virtual PatternMatchingRank rankToMatchType(const TypePtr &type);

    /// This method computes the rank required for matching the specified value without implicit casting.
    virtual PatternMatchingRank rankToMatchValue(const AnyValuePtr &value);

protected:
    virtual AnyValuePtr lookupLocalSelector(const AnyValuePtr &selector);

    TypePtr supertype;

    std::unordered_map<AnyValuePtr, AnyValuePtr> methodDictionary;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP