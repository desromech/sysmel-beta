#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_UNION_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_UNION_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeUnionLayout);


/**
 * I represent the layout of an aggregate type whose elements all share the same position.
 */
class SYSMEL_COMPILER_LIB_EXPORT AggregateTypeUnionLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeUnionLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeUnionLayout";

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_UNION_LAYOUT_HPP