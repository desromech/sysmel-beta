#include "Environment/CompilationError.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompilationError> CompilationErrorTypeRegistration;

} // End of namespace Environment
} // End of namespace Sysmel