#ifndef SYSMEL_ENVIRONMENT_TYPE_CONVERSION_RULE_HPP
#define SYSMEL_ENVIRONMENT_TYPE_CONVERSION_RULE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TypeConversionRule);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT TypeConversionRule : public SubtypeOf<CompilerObject, TypeConversionRule>
{
public:
    static constexpr char const __typeName__[] = "TypeConversionRule";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const;
    virtual TypeConversionCost getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const;
    virtual ASTNodePtr convertNodeAtIntoWith(const ASTNodePtr &node, const ASTSourcePositionPtr &sourcePosition, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_FUNCTION_TYPE_HPP