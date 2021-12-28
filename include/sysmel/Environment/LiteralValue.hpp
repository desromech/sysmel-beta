#ifndef SYSMEL_ENVIRONMENT_LITERAL_VALUE_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_VALUE_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralValue);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class SYSMEL_COMPILER_LIB_EXPORT LiteralValue : public SubtypeOf<AnyValue, LiteralValue>
{
public:
    static constexpr char const __typeName__[] = "LiteralValue";
    static constexpr char const __sysmelTypeName__[] = "LiteralValue";
    static constexpr bool __isImmutableType__ = true;

    static void __addTypeConversionRules__(const TypePtr &type);
    static TypePtr __asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment);

    virtual bool isLiteralValue() const override;
    virtual bool isPureCompileTimeLiteralValue() const override;

    virtual bool canBeConcretizedWithType(const TypePtr &targetType);
    virtual AnyValuePtr concretizeWithType(const TypePtr &targetType);

    virtual TypePtr getStandardConcreteType();
    virtual TypePtr getBestConcreteType();
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_VALUE_HPP