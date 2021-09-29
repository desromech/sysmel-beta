#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SPECIFIC_METHOD_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SPECIFIC_METHOD_HPP
#pragma once

#include "Method.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am method signature.
 */
struct MethodSignature
{
    TypePtr resultType;
    TypePtr receiverType;
    TypePtrList argumentTypes;
};

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class SpecificMethod : public SubtypeOf<Method, SpecificMethod>
{
public:
    static constexpr char const __typeName__[] = "SpecificMethod";

    virtual bool isSpecificMethod() const override;

    const MethodSignature &getSignature() const
    {
        return signature;
    }
    
    virtual MethodPatternMatchingResult matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;

    virtual bool isMacroMethod() const override;

    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

protected:
    MethodSignature signature;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SPECIFIC_METHOD_HPP