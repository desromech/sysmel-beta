#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/BootstrapModule.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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


} // End of namespace ObjectModel
} // End of namespace SysmelMoebius