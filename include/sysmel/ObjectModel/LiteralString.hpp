#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_STRING_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_STRING_HPP

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a literal string value.
 */
class LiteralString : public LiteralValue
{
public:
    LiteralString(const std::string &ctorValue = std::string())
        : value(ctorValue) {}
        
    virtual bool isLiteralString() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;

protected:
    std::string value;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_STRING_HPP