#ifndef SYSMEL_ENVIRONMENT_METABUILDER_FACTORY_HPP
#define SYSMEL_ENVIRONMENT_METABUILDER_FACTORY_HPP
#pragma once

#include "BootstrapMethod.hpp"
#include "MacroInvocationContext.hpp"
#include "MetaBuilderInstanceContext.hpp"

namespace Sysmel
{
namespace Environment
{

template<typename T>
MethodPtr metaBuilderFactoryFor (const std::string &name)
{
    return makeBootstrapMethod<ObjectPtr<T> (MacroInvocationContextPtr)> (name, [](const MacroInvocationContextPtr &macroContext) {
        auto context = basicMakeObject<MetaBuilderInstanceContext> ();
        context->instanceNode = macroContext->receiverNode;

        auto result = basicMakeObject<T> ();
        result->setMetaBuilderInstanceContext(context);
        return result;
    }, MethodFlags::Macro);
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_METABUILDER_FACTORY_HPP
