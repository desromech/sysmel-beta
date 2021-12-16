#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_LAYOUT_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeLayout);


/**
 * I represent the layout of an aggregate type.
 */
class AggregateTypeLayout : public SubtypeOf<CompilerObject, AggregateTypeLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeLayout";

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_LAYOUT_HPP