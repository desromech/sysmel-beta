#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_CONVERSION_RULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_CONVERSION_RULE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TypeConversionRule);

/**
 * I am an instance of a function type object.
 */
class TypeConversionRule : public SubtypeOf<CompilerObject, TypeConversionRule>
{
public:
    static constexpr char const __typeName__[] = "TypeConversionRule";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool canBeUsedToConvertNodeFromTo(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType) const;
    virtual size_t getConversionCost(const ASTNodePtr &node, const TypePtr &targetType) const;
    virtual ASTNodePtr convertNodeIntoWith(const ASTNodePtr &node, const TypePtr &targetType, const ASTSemanticAnalyzerPtr &semanticAnalyzer) const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP