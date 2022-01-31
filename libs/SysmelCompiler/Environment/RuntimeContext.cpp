#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapModule.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/Type.hpp"
#include "Frontend/SysmelSyntax/SysmelLanguageSupport.hpp"
#include <iostream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<RuntimeContext> runtimeContextTypeRegistration;

static thread_local RuntimeContextPtr activeRuntimeContextInThisThread;

RefCountedObjectMemoryPool *RefCountedObjectMemoryPool::getActive()
{
    return RuntimeContext::getActive()->getContextMemoryPool();
}

RuntimeContextPtr RuntimeContext::createForTarget(const RuntimeContextTargetDescription &target)
{
    auto result = basicMakeGlobalSingletonObject<RuntimeContext> ();
    result->target = target;
    result->activeDuring([&](){
        result->initialize();
    });
    return result;
}

RuntimeContextPtr RuntimeContext::createForScripting()
{
    return createForTarget(RuntimeContextTargetDescription::makeForScripting());
}

void RuntimeContext::initialize()
{
    bootstrapModule = basicMakeObject<BootstrapModule> ();
    bootstrapModule->activeDuring([&](){
        bootstrapModule->initialize();

        sysmelLanguageSupport = basicMakeObject<SysmelLanguageSupport> ();
        sysmelLanguageSupport->initialize();
    });
}

RuntimeContextPtr RuntimeContext::getActive()
{
    return activeRuntimeContextInThisThread;
}

void RuntimeContext::setActive(const RuntimeContextPtr &aRuntimeContext)
{
    activeRuntimeContextInThisThread = aRuntimeContext;
}

TypeConversionRulePtr RuntimeContext::findCachedTypeConversionRuleFor(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType, bool isExplicit)
{
    if(node && !node->isValidForCachingTypeConversionRules())
        return nullptr;

    auto dictionary = isExplicit ? &explicitTypeConversionRuleCache : &implicitTypeConversionRuleCache;
    auto it = dictionary->find({sourceType, targetType});
    return it != dictionary->end() ? it->second : nullptr;
}

void RuntimeContext::setCachedTypeConversionRuleFor(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType, bool isExplicit, const TypeConversionRulePtr &rule)
{
    if(node && !node->isValidForCachingTypeConversionRules())
        return;

    auto dictionary = isExplicit ? &explicitTypeConversionRuleCache : &implicitTypeConversionRuleCache;
    dictionary->insert({{sourceType, targetType}, rule});
}

} // End of namespace Environment
} // End of namespace Sysmel