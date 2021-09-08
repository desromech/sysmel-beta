#ifndef SYSMEL_COMPILER_OBJECT_MODEL_PATTERN_MATCHING_METHOD_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_PATTERN_MATCHING_METHOD_HPP
#pragma once

#include "Method.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

    AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;

protected:
    std::vector<MethodPtr> patterns;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_PATTERN_MATCHING_METHOD_HPP