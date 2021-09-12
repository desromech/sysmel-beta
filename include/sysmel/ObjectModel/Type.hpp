#ifndef SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP
#pragma once

#include "ProgramEntity.hpp"

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

    static MethodCategories __instanceMethods__();

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual TypePtr getSupertype();

    /// This method sets the super type.
    virtual void setSupertype(const TypePtr &newSupertype);

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr lookupSelector(const AnyValuePtr &selector);

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);

    /// This method add a new macro method into the method dictionary with the specified selector.
    virtual void addMacroMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method);

    /// This method adds the method in the specified categories.
    virtual void addMacroMethodCategories(const MethodCategories &categories);

    /// This method add a new method into the method dictionary with the specified selector.
    virtual void addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method);

    /// This method adds the method in the specified categories.
    virtual void addMethodCategories(const MethodCategories &categories);

    /// Is this type a kind of the other type?
    virtual bool isKindOf(const TypePtr &otherType);

    /// This method computes the rank required for matching the specified type without implicit casting.
    virtual PatternMatchingRank rankToMatchType(const TypePtr &type);

    /// This method computes the rank required for matching the specified value without implicit casting.
    virtual PatternMatchingRank rankToMatchValue(const AnyValuePtr &value);

    /// This method returns the instance type.
    virtual TypePtr getInstanceType();

    /// This method returns the meta type.
    virtual TypePtr getMetaType();

protected:
    virtual AnyValuePtr lookupLocalSelector(const AnyValuePtr &selector);

    TypePtr supertype;

    MethodDictionaryPtr macroMethodDictionary;
    MethodDictionaryPtr methodDictionary;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_TYPE_HPP