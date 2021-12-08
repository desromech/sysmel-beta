#ifndef SYSMEL_ENVIRONMENT_SIMPLE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_SIMPLE_TYPE_HPP
#pragma once

#include "Type.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base implementation for a simple type
 */
class SimpleType : public SubtypeOf<Type, SimpleType>
{
public:
    static constexpr char const __typeName__[] = "SimpleType";

    virtual bool isSimpleType() const override;

    virtual TypePtr getType() const override;
    void setType(const TypePtr &theMetaType);

    void setSupertypeAndImplicitMetaType(const TypePtr &newSupertype);

protected:

    TypePtr metaType;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SIMPLE_TYPE_HPP