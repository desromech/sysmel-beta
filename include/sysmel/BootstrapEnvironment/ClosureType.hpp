#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLOSURE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLOSURE_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureTypeValue);

/**
 * I am an instance of a function type object.
 */
class ClosureType : public SubtypeOf<FunctionalType, ClosureType>
{
public:
    virtual bool isClosureType() const override;

};

/**
 * I am an instance of a function type object.
 */
class ClosureTypeValue : public SubtypeOf<FunctionalTypeValue, ClosureTypeValue>
{
public:
    virtual bool isClosureTypeValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLOSURE_TYPE_HPP