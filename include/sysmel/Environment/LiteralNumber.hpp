#ifndef SYSMEL_ENVIRONMENT_LITERAL_NUMBER_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_NUMBER_HPP
#pragma once

#include "LiteralValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralNumber);


/**
 * I am literal number value.
 */
class SYSMEL_COMPILER_LIB_EXPORT LiteralNumber : public SubtypeOf<LiteralValue, LiteralNumber>
{
public:
    static constexpr char const __typeName__[] = "LiteralNumber";
    static constexpr char const __sysmelTypeName__[] = "LiteralNumber";

    static MethodCategories __instanceMacroMethods__();
    static MethodCategories __instanceMethods__();

    static TypePtr __asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment);

    virtual bool isLiteralNumber() const override;

    virtual double asFloat() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_NUMBER_HPP