#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeSequentialLayout);


/**
 * I represent the layout of an aggregate type whose elements are laid on sequentially.
 */
class AggregateTypeSequentialLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeSequentialLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeSequentialLayout";

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP