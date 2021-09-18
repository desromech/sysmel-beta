#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_ARRAY_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_ARRAY_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class LiteralArray;
typedef std::shared_ptr<LiteralArray> LiteralArrayPtr;


/**
 * I am literal number value.
 */
class LiteralArray : public SubtypeOf<LiteralValue, LiteralArray>
{
public:
    static constexpr char const __typeName__[] = "LiteralArray";

    static LiteralArrayPtr makeFor(const AnyValuePtrList &content);

    static MethodCategories __instanceMethods__();

    virtual bool isLiteralArray() const override;

    virtual AnyValuePtrList unwrapAsArray() const override;

    virtual std::string printString() const override;

protected:
    AnyValuePtrList content;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_ARRAY_HPP