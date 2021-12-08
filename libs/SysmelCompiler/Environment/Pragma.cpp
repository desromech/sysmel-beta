#include "Environment/Pragma.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<Pragma> pragmaTypeRegistration;

TypePtr Type::getPragmaType()
{
    return Pragma::__staticType__();
}

bool Pragma::isPragma() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel