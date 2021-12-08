#ifndef SYSMEL_ENVIRONMENT_DOWNCAST_CONVERSION_RULE_HPP
#define SYSMEL_ENVIRONMENT_DOWNCAST_CONVERSION_RULE_HPP
#pragma once

#include "TypeConversionRule.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an instance of a function type object.
 */
class DowncastTypeConversionRule : public SubtypeOf<TypeConversionRule, DowncastTypeConversionRule>
{
public:
    static constexpr char const __typeName__[] = "DowncastTypeConversionRule";

    static TypeConversionRulePtr uniqueInstance();

    virtual bool canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const override;
    virtual size_t getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const override;
    virtual ASTNodePtr convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_UPCAST_CONVERSION_RULE_HPP