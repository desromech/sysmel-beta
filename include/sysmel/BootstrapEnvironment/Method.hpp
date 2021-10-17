#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Method);

struct MethodPatternMatchingResult
{
    MethodPtr matchingMethod;
    PatternMatchingRank matchingRank;
};

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class Method : public SubtypeOf<ModuleDefinedProgramEntity, Method>
{
public:
    static constexpr char const __typeName__[] = "Method";

    virtual bool isMethod() const override;

    virtual AnyValuePtr getName() const override;
    virtual void setName(const AnyValuePtr &newName);

    virtual TypePtr getExpectedTypeForAnalyzingArgumentWithIndex(size_t argumentIndex);
    virtual MethodPatternMatchingResult matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);
    virtual MethodPatternMatchingResult matchPatternForAnalyzingMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

protected:
    AnyValuePtr name;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_HPP