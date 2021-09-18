#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_BOOLEAN_TRUE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_BOOLEAN_TRUE_HPP
#pragma once

#include "LiteralBoolean.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class LiteralBooleanTrue;
typedef std::shared_ptr<LiteralBooleanTrue> LiteralBooleanTruePtr;


/**
 * I am literal number value.
 */
class LiteralBooleanTrue : public SubtypeOf<LiteralBoolean, LiteralBooleanTrue>
{
public:
    static constexpr char const __typeName__[] = "LiteralBooleanTrue";

    static LiteralBooleanTruePtr uniqueInstance();

    virtual bool isLiteralBooleanTrue() const override;

    virtual bool unwrapAsBoolean() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_BOOLEAN_TRUE_HPP