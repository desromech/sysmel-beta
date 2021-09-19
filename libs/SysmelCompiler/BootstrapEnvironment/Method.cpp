#include "sysmel/BootstrapEnvironment/Method.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Method> methodTypeRegistration;

bool Method::isMethod() const
{
    return true;
}

MethodPatternMatchingResult Method::matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    (void)arguments;
    (void)receiver;
    return MethodPatternMatchingResult{};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius