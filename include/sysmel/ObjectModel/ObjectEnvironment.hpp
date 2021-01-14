#ifndef SYSMEL_OBJECT_MODEL_OBJECT_ENVIRONMENT_HPP
#define SYSMEL_OBJECT_MODEL_OBJECT_ENVIRONMENT_HPP
#pragma once

#include "SObjectSchema.hpp"
#include "CompilationTarget.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

struct ObjectEnvironment;
typedef std::shared_ptr<ObjectEnvironment> ObjectEnvironmentPtr;

/**
 * The interface for an object environment.
 */
struct ObjectEnvironment : Object
{
    SYSMEL_CLASS_DECLARE(ObjectEnvironment, Object);

    virtual CompilationTargetPtr getCompilationTarget() const = 0;
    virtual SchemaPtr getSchema() const = 0;
};

/**
 * An standard object environment
 */
class StandardObjectEnvironment : public ObjectEnvironment
{
public:
    StandardObjectEnvironment();

    virtual CompilationTargetPtr getCompilationTarget() const override;
    virtual SchemaPtr getSchema() const override;

    CompilationTargetPtr compilationTarget;
    SchemaPtr schema;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_OBJECT_ENVIRONMENT_HPP
