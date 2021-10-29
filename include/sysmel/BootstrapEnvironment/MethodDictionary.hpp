#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_DICTIONARY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_DICTIONARY_HPP
#pragma once

#include "CompilerObject.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class MethodDictionary : public SubtypeOf<CompilerObject, MethodDictionary>
{
public:
    static constexpr char const __typeName__[] = "MethodDictionary";

    virtual bool isMethodDictionary() const override;

    void addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector);
    AnyValuePtr lookupSelector(const AnyValuePtr &selector);

protected:
    std::unordered_map<AnyValuePtr, AnyValuePtr> dictionary;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_DICTIONARY_HPP