#include "sysmel/BootstrapEnvironment/Variable.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Variable> variableTypeRegistration;

bool Variable::isVariable() const
{
    return true;
}

void Variable::setDefinitionParameters(const AnyValuePtr &definitionName, const TypePtr &definitionValueType, TypeInferenceMode typeInferenceMode, bool definitionMutability, uint64_t definitionMinimalAlignment)
{
    name = definitionName;
    valueType = definitionValue;
    isMutable_ = definitionMutability;
    minimalAlignment = definitionMinimalAlignment;
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius