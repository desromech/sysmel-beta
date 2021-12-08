#include "Environment/LiteralValue.hpp"
#include "Environment/LiteralToTargetTypeConversionRule.hpp"
#include "Environment/Type.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
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
    return targetType->canBeInstantiatedWithLiteralValue(selfFromThis());
}

AnyValuePtr LiteralValue::concretizeWithType(const TypePtr &targetType)
{
    return targetType->instantiatedWithLiteralValue(selfFromThis());
}

TypePtr LiteralValue::getStandardConcreteType()
{
    return getType();
}

TypePtr LiteralValue::getBestConcreteType()
{
    return getType();
}

} // End of namespace Environment
} // End of namespace Sysmel