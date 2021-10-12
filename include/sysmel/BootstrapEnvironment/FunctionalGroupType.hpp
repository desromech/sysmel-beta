#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_GROUP_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_GROUP_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionGroupType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class FunctionGroupType : public SubtypeOf<FunctionalType, FunctionGroupType>
{
public:
    static constexpr char const __typeName__[] = "FunctionGroupType";
    static constexpr char const __sysmelTypeName__[] = "_FunctionGroupType";

    virtual bool isFunctionGroupType() const override;

    ProgramEntityPtr group;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_GROUP_TYPE_HPP