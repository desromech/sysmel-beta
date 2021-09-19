#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<RuntimeContext> runtimeContextTypeRegistration;

static thread_local RuntimeContextPtr activeRuntimeContextInThisThread;


RuntimeContextPtr RuntimeContext::create()
{
    auto result = std::make_shared<RuntimeContext> ();
    result->activeDuring([&](){
        result->initialize();
    });
    return result;
}

void RuntimeContext::initialize()
{
    bootstrapModule = std::make_shared<BootstrapModule> ();
    bootstrapModule->activeDuring([&](){
        bootstrapModule->initialize();
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