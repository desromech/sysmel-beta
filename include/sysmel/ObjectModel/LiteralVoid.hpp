#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_VOID_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_VOID_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class LiteralVoid;
typedef std::shared_ptr<LiteralVoid> LiteralVoidPtr;


/**
 * I am literal number value.
 */
class LiteralVoid : public SubtypeOf<LiteralValue, LiteralVoid>
{
public:
    static constexpr char const __typeName__[] = "LiteralVoid";
    
    static LiteralVoidPtr uniqueInstance();

    virtual bool isLiteralVoid() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_VOID_HPP