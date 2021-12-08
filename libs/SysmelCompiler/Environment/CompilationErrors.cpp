#include "Environment/CompilationErrors.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel