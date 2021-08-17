#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_INTEGER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_INTEGER_HPP

#include "LiteralNumber.hpp"
#include "LargeInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class LiteralInteger;
typedef std::shared_ptr<LiteralInteger> LiteralIntegerPtr;

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralInteger : public LiteralNumber
{
public:
    static LiteralIntegerPtr makeFor(const LargeInteger &value);
    static LiteralIntegerPtr makeFor(LargeInteger &&value);
    static LiteralIntegerPtr makeForCharacter(char32_t value);

    virtual bool isLiteralInteger() const override;
    virtual std::string printString() const override;

protected:
    LargeInteger value;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_INTEGER_HPP