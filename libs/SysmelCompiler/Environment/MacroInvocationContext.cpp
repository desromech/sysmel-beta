#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/BootstrapFieldVariable.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<MacroInvocationContext> macroInvocationContextRegistration;

FieldVariablePtrList MacroInvocationContext::__fieldVariables__()
{
    return FieldVariablePtrList{
        makeBootstrapFieldVariable("self", &SelfType::receiverNode),
        makeBootstrapFieldVariable("SelfType", &SelfType::selfType),
        makeBootstrapFieldVariable("__astBuilder", &SelfType::astBuilder),
    };
}

bool MacroInvocationContext::isMacroInvocationContext() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel