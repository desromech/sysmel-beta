#ifndef SYSMEL_COMPILER_OBJECT_MODEL_METHOD_DICTIONARY_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_METHOD_DICTIONARY_HPP
#pragma once

#include "CompilerObject.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace ObjectModel
{
/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class MethodDictionary : public SubtypeOf<CompilerObject, MethodDictionary>
{
public:
    static constexpr char const __typeName__[] = "MethodDictionary";

    virtual bool isMethodDictionary() const override;

    void addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method);
    AnyValuePtr lookupSelector(const AnyValuePtr &selector);

protected:
    std::unordered_map<AnyValuePtr, AnyValuePtr> dictionary;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_METHOD_DICTIONARY_HPP