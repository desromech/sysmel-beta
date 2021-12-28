#ifndef SYSMEL_ENVIRONMENT_METHOD_DICTIONARY_HPP
#define SYSMEL_ENVIRONMENT_METHOD_DICTIONARY_HPP
#pragma once

#include "CompilerObject.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class SYSMEL_COMPILER_LIB_EXPORT MethodDictionary : public SubtypeOf<CompilerObject, MethodDictionary>
{
public:
    static constexpr char const __typeName__[] = "MethodDictionary";

    virtual bool isMethodDictionary() const override;

    void addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector);
    AnyValuePtr lookupSelector(const AnyValuePtr &selector);

protected:
    std::unordered_map<AnyValuePtr, AnyValuePtr> dictionary;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_METHOD_DICTIONARY_HPP