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
class SYSMEL_COMPILER_LIB_EXPORT PatternMatchingMethod : public SubtypeOf<Method, PatternMatchingMethod>
{
public:
    static constexpr char const __typeName__[] = "PatternMatchingMethod";

    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isLiteralValueMessageAnalyzer__ = true;

    PatternMatchingMethod(const AnyValuePtr &initialName);

    virtual bool isPatternMatchingMethod() const override;

    void addPattern(const MethodPtr &newPattern);

    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual AnyValuePtr asMethodMatchingDefinitionSignature(bool hasReceiver, bool hasConstReceiver, const TypePtrList &argumentTypes, const TypePtr &resultType) override;
    virtual AnyValuePtr asMethodMatchingSignature(const TypePtr &receiverType, const TypePtrList &argumentTypes, const TypePtr &resultType) override;


protected:
    void ensureArgumentsAreAnalyzed(ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    
    std::vector<MethodPtr> patterns;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PATTERN_MATCHING_METHOD_HPP