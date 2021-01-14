#ifndef SYSMEL_OBJECT_MODEL_OBJECT_ENVIRONMENT_BUILDERS_HPP
#define SYSMEL_OBJECT_MODEL_OBJECT_ENVIRONMENT_BUILDERS_HPP
#pragma once

#include "ObjectEnvironment.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * The interface for an object environment.
 */
struct ObjectEnvironmentTypeMetaBuilder : Object
{
    SYSMEL_CLASS_DECLARE(ObjectEnvironmentTypeMetaBuilder, Object);

    virtual ObjectPtr performWithArguments(const std::string &selector, const ObjectPtrList &arguments) override;

    ObjectEnvironmentPtr objectEnvironment;
};


} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_OBJECT_ENVIRONMENT_BUILDERS_HPP
