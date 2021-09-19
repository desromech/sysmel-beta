#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_SYMBOL_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_SYMBOL_HPP
#pragma once

#include "LiteralString.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class LiteralSymbol;
typedef std::shared_ptr<LiteralSymbol> LiteralSymbolPtr;

/**
 * I am a literal symbol value.
 */
class LiteralSymbol : public SubtypeOf<LiteralString, LiteralSymbol>
{
public:
    static constexpr char const __typeName__[] = "LiteralSymbol";

    static LiteralSymbolPtr makeFor(const std::string &value);
    static LiteralSymbolPtr intern(const std::string &value);
    
    virtual bool isLiteralSymbol() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

typedef std::shared_ptr<LiteralSymbol> LiteralSymbolPtr;

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_SYMBOL_HPP