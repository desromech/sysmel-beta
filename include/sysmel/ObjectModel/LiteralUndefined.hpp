#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_UNDEFINED_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_UNDEFINED_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class LiteralUndefined;
typedef std::shared_ptr<LiteralUndefined> LiteralUndefinedPtr;


/**
 * I am literal number value.
 */
class LiteralUndefined : public SubtypeOf<LiteralValue, LiteralUndefined>
{
public:
    static constexpr char const __typeName__[] = "LiteralUndefined";

    static LiteralUndefinedPtr uniqueInstance();

    virtual bool isLiteralUndefined() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_UNDEFINED_HPP