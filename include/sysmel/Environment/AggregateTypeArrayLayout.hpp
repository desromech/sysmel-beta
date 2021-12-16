#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_ARRAY_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_ARRAY_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeArrayLayout);


/**
 * I represent the layout of an array aggregate type.
 */
class AggregateTypeArrayLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeArrayLayout";

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_ARRAY_LAYOUT_HPP