#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_HPP
#pragma once

#include "ProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class Method;
typedef std::shared_ptr<Method> MethodPtr;

struct MethodPatternMatchingResult
{
    MethodPtr matchingMethod;
    PatternMatchingRank matchingRank;
};

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class Method : public SubtypeOf<ProgramEntity, Method>
{
public:
    static constexpr char const __typeName__[] = "Method";

    virtual bool isMethod() const override;

    const AnyValuePtr &getName() const
    {
        return name;
    }

    virtual MethodPatternMatchingResult matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);

protected:
    AnyValuePtr name;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_HPP