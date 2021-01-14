#ifndef SYSMEL_OBJECT_MODEL_COMPILATION_TARGET_HPP
#define SYSMEL_OBJECT_MODEL_COMPILATION_TARGET_HPP
#pragma once

#include "Object.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

struct CompilationTarget;
typedef std::shared_ptr<CompilationTarget> CompilationTargetPtr;

/**
 * The interface for a compilation target
 */
struct CompilationTarget : Object
{
    SYSMEL_CLASS_DECLARE(CompilationTarget, Object);
    
    virtual std::string getName() const = 0;
    virtual size_t getPointerAlignment() const = 0;
    virtual size_t getFloat64Alignment() const = 0;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_COMPILATION_TARGET_HPP
