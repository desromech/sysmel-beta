#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_BOOLEAN_FALSE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_BOOLEAN_FALSE_HPP
#pragma once

#include "LiteralBoolean.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class LiteralBooleanFalse;
typedef std::shared_ptr<LiteralBooleanFalse> LiteralBooleanFalsePtr;


/**
 * I am literal number value.
 */
class LiteralBooleanFalse : public SubtypeOf<LiteralBoolean, LiteralBooleanFalse>
{
public:
    static constexpr char const __typeName__[] = "LiteralBooleanFalse";

    static LiteralBooleanFalsePtr uniqueInstance();

    virtual bool isLiteralBooleanFalse() const override;
    
    virtual bool unwrapAsBoolean() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_BOOLEAN_FALSE_HPP