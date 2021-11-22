#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONVERSION_METHOD_TYPE_CONVERSION_RULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONVERSION_METHOD_TYPE_CONVERSION_RULE_HPP
#pragma once

#include "TypeConversionRule.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Method);

/**
 * I am an instance of a function type object.
 */
class ConversionMethodTypeConversionRule : public SubtypeOf<TypeConversionRule, ConversionMethodTypeConversionRule>
{
public:
    static constexpr char const __typeName__[] = "ConversionMethodTypeConversionRule";

    static TypeConversionRulePtr makeFor(const TypePtr &sourceType, const TypePtr &destinationType, const MethodPtr &method);

    virtual bool canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const override;
    virtual size_t getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const override;
    virtual ASTNodePtr convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const override;

    TypePtr sourceType;
    TypePtr destinationType;
    MethodPtr method;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CONVERSION_METHOD_TYPE_CONVERSION_RULE_HPP