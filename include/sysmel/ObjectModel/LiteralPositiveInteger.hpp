#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_POSITIVE_INTEGER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_POSITIVE_INTEGER_HPP

#include "LiteralInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a literal positive integer value.
 */
class LiteralPositiveInteger : public LiteralInteger
{
public:
    virtual bool isLiteralPositiveInteger() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_POSITIVE_INTEGER_HPP