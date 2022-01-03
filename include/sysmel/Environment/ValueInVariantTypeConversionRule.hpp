#ifndef SYSMEL_ENVIRONMENT_VALUE_IN_VARIANT_TYPE_CONVERSION_RULE_HPP
#define SYSMEL_ENVIRONMENT_VALUE_IN_VARIANT_TYPE_CONVERSION_RULE_HPP
#pragma once

#include "TypeConversionRule.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I a type conversion rule that is used for boxing values in variants.
 */
class SYSMEL_COMPILER_LIB_EXPORT ValueInVariantTypeConversionRule : public SubtypeOf<TypeConversionRule, ValueInVariantTypeConversionRule>
{
public:
    static constexpr char const __typeName__[] = "ValueInVariantTypeConversionRule";

    static TypeConversionRulePtr uniqueInstance();

    virtual bool canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const override;
    virtual size_t getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const override;
    virtual ASTNodePtr convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_VALUE_IN_VARIANT_TYPE_CONVERSION_RULE_HPP