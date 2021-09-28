#include "sysmel/BootstrapEnvironment/CompilationErrors.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CompilationErrors> CompilationErrorsTypeRegistration;

std::string CompilationErrors::getDescription() const
{
    std::ostringstream out;
    bool first = true;
    for(const auto &error : errors)
    {
        if(first)
            first = false;
        else
            out << '\n';

        out << error->getDescription();
    }

    return out.str();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius