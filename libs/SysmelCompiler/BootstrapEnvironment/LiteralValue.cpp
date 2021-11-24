#include "sysmel/BootstrapEnvironment/LiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/LiteralToTargetTypeConversionRule.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>
#include <iostream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralValue> literalValueTypeRegistration;

TypePtr Type::getLiteralValueType()
{
    return LiteralValue::__staticType__();
}

void LiteralValue::__addTypeConversionRules__(const TypePtr &type)
{
    type->addTypeConversionRule(LiteralToTargetTypeConversionRule::uniqueInstance());
}

TypePtr LiteralValue::__asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    if(concreteLiterals && node->isASTLiteralValueNode())
    {
        auto result = environment->literalValueInferrenceType;
        if(result && !result->isVoidType())
            return result;
    }

    return SuperType::__asInferredTypeForWithModeInEnvironment__(selfType, node, mode, isMutable, concreteLiterals, environment);
}

bool LiteralValue::isLiteralValue() const
{
    return true;
}

bool LiteralValue::isPureCompileTimeLiteralValue() const
{
    return true;
}

bool LiteralValue::canBeConcretizedWithType(const TypePtr &targetType)
{
    return targetType->canBeInstantiatedWithLiteralValue(shared_from_this());
}

AnyValuePtr LiteralValue::concretizeWithType(const TypePtr &targetType)
{
    return targetType->instantiatedWithLiteralValue(shared_from_this());
}

TypePtr LiteralValue::getStandardConcreteType()
{
    return getType();
}

TypePtr LiteralValue::getBestConcreteType()
{
    return getType();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius