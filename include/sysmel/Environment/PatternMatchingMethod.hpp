#ifndef SYSMEL_ENVIRONMENT_PATTERN_MATCHING_METHOD_HPP
#define SYSMEL_ENVIRONMENT_PATTERN_MATCHING_METHOD_HPP
#pragma once

#include "Method.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class PatternMatchingMethod : public SubtypeOf<Method, PatternMatchingMethod>
{
public:
    static constexpr char const __typeName__[] = "PatternMatchingMethod";

    PatternMatchingMethod(const AnyValuePtr &initialName);

    virtual bool isPatternMatchingMethod() const override;

    void addPattern(const MethodPtr &newPattern);

    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

protected:
    void ensureArgumentsAreAnalyzed(ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    
    std::vector<MethodPtr> patterns;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PATTERN_MATCHING_METHOD_HPP