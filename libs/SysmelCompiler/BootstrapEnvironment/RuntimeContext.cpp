#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/Compiler/Sysmel/SysmelLanguageSupport.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius