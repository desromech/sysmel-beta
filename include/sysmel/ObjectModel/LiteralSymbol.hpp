#ifndef SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_SYMBOL_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_SYMBOL_HPP

#include "LiteralString.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a literal symbol value.
 */
class LiteralSymbol : public LiteralString
{
public:
    static std::shared_ptr<LiteralSymbol> intern(const std::string &value);
    
    virtual bool isLiteralSymbol() const override;

    virtual std::string printString() const override;

private:
    LiteralSymbol(const std::string &ctorValue = std::string())
        : LiteralString(ctorValue) {}
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_LITERAL_SYMBOL_HPP