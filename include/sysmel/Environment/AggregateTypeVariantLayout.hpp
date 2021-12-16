#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_VARIANT_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_VARIANT_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeVariantLayout);


/**
 * I represent the layout of an aggregate type with single discriminated element.
 */
class AggregateTypeVariantLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeVariantLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeVariantLayout";

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_VARIANT_LAYOUT_HPP