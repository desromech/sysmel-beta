#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NEGATIVE_INTEGER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NEGATIVE_INTEGER_HPP

#include "LiteralInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a literal negative integer value.
 */
class LiteralNegativeInteger : public LiteralInteger
{
public:
    virtual bool isLiteralNegativeInteger() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NEGATIVE_INTEGER_HPP