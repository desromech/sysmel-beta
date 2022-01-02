#ifndef SYSMEL_ENVIRONMENT_PADDING_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PADDING_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PaddingType);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT PaddingType : public SubtypeOf<SimpleType, PaddingType>
{
public:
    static PaddingTypePtr make(uint64_t size);

    virtual bool isPaddingType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &typeVisitor) override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;

    uint64_t size;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PADDING_TYPE_HPP