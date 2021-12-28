#ifndef SYSMEL_ENVIRONMENT_BOOTSTRAP_EXTENSION_METHODS_HPP
#define SYSMEL_ENVIRONMENT_BOOTSTRAP_EXTENSION_METHODS_HPP
#pragma once

#include "AnyValue.hpp"
#include <functional>

namespace Sysmel
{
namespace Environment
{

typedef std::function<MethodCategories ()> BootstrapExtensionMethodsDescriptionFunction;

/**
 * I provide facilities for building a list of bootstrap extension methods
 */
class SYSMEL_COMPILER_LIB_EXPORT BootstrapExtensionMethods
{
public:

    void applyToType(const TypePtr &type) const;

    BootstrapExtensionMethods &instanceMethods(const BootstrapExtensionMethodsDescriptionFunction &methods)
    {
        instanceMethodsDescriptionFunction = methods;
        return *this;
    }

    BootstrapExtensionMethods &instanceMacroMethods(const BootstrapExtensionMethodsDescriptionFunction &macroMethods)
    {
        instanceMacroMethodsDescriptionFunction = macroMethods;
        return *this;
    }

    BootstrapExtensionMethods &typeMethods(const BootstrapExtensionMethodsDescriptionFunction &methods)
    {
        typeMethodsDescriptionFunction = methods;
        return *this;
    }

    BootstrapExtensionMethods &typeMacroMethods(const BootstrapExtensionMethodsDescriptionFunction &macroMethods)
    {
        typeMacroMethodsDescriptionFunction = macroMethods;
        return *this;
    }

    MethodCategories getInstanceMethods() const
    {
        return instanceMethodsDescriptionFunction();
    }

    MethodCategories getInstanceMacroMethods() const
    {
        return instanceMacroMethodsDescriptionFunction();
    }

    MethodCategories getTypeMethods() const
    {
        return typeMethodsDescriptionFunction();
    }

    MethodCategories getTypeMacroMethods() const
    {
        return typeMacroMethodsDescriptionFunction();
    }

private:
    BootstrapExtensionMethodsDescriptionFunction instanceMethodsDescriptionFunction;
    BootstrapExtensionMethodsDescriptionFunction instanceMacroMethodsDescriptionFunction;

    BootstrapExtensionMethodsDescriptionFunction typeMethodsDescriptionFunction;
    BootstrapExtensionMethodsDescriptionFunction typeMacroMethodsDescriptionFunction;
};

typedef std::pair<StaticBootstrapDefinedTypeMetadata*, BootstrapExtensionMethods> BootstrapTypeWithExtensionMethodsPair;
typedef std::vector<BootstrapTypeWithExtensionMethodsPair> BootstrapExtensionMethodsList;

SYSMEL_COMPILER_LIB_EXPORT const BootstrapExtensionMethodsList &getRegisteredBootstrapExtensionMethods();
SYSMEL_COMPILER_LIB_EXPORT void registerBootstrapExtensionMethods(StaticBootstrapDefinedTypeMetadata *typeMetadata, const BootstrapExtensionMethods &extensionMethods);

template<typename T>
class BootstrapExtensionMethodsFor
{
public:
    BootstrapExtensionMethodsFor(const BootstrapExtensionMethods &extensionMethods)
    {
        registerBootstrapExtensionMethods(StaticBootstrapDefinedTypeFor<T>::get(), extensionMethods);
    }
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_BOOTSTRAP_EXTENSION_METHODS_HPP