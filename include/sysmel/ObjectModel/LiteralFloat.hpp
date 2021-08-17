#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FLOAT_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FLOAT_HPP

#include "LiteralNumber.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class LiteralFloat : public LiteralNumber
{
public:
    LiteralFloat(double ctorValue)
        : value(ctorValue) {}

    virtual bool isLiteralFloat() const override;

    virtual std::string printString() const override;

protected:
    double value;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_FLOAT_HPP