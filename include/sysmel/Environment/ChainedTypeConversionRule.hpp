#ifndef SYSMEL_ENVIRONMENT_CHAINED_TYPE_CONVERSION_RULE_HPP
#define SYSMEL_ENVIRONMENT_CHAINED_TYPE_CONVERSION_RULE_HPP
#pragma once

#include "TypeConversionRule.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ChainedTypeConversionRule : public SubtypeOf<TypeConversionRule, ChainedTypeConversionRule>
{
public:
    static constexpr char const __typeName__[] = "ChainedTypeConversionRule";

    virtual bool canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const override;
    virtual TypeConversionCost getConversionCost(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const override;
    virtual ASTNodePtr convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const override;

    virtual TypePtr getCanonicalTargetTypeFor(const TypePtr &sourceType) const override;

    TypeConversionRulePtr firstConversionRule;
    TypePtr intermediateType;
    TypeConversionRulePtr secondConversionRule;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CHAINED_TYPE_CONVERSION_RULE_HPP