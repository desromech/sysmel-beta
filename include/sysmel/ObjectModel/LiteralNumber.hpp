#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NUMBER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NUMBER_HPP

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am literal number value.
 */
class LiteralNumber : public LiteralValue
{
public:
    virtual bool isLiteralNumber() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_NUMBER_HPP