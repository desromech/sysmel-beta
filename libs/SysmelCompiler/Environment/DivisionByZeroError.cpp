#include "Environment/DivisionByZeroError.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<DivisionByZeroError> DivisionByZeroErrorTypeRegistration;

} // End of namespace Environment
} // End of namespace Sysmel