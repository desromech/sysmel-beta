#include "Environment/ArgumentCountError.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ArgumentCountError> ArgumentCountErrorTypeRegistration;

} // End of namespace Environment
} // End of namespace Sysmel