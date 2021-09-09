#include "sysmel/ObjectModel/Method.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius