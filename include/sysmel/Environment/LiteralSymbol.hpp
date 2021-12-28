#ifndef SYSMEL_ENVIRONMENT_LITERAL_SYMBOL_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_SYMBOL_HPP
#pragma once

#include "LiteralString.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralSymbol);

/**
 * I am a literal symbol value.
 */
class SYSMEL_COMPILER_LIB_EXPORT LiteralSymbol : public SubtypeOf<LiteralString, LiteralSymbol>
{
public:
    static constexpr char const __typeName__[] = "LiteralSymbol";
    static constexpr char const __sysmelTypeName__[] = "LiteralSymbol";

    static LiteralSymbolPtr makeFor(const std::string &value);
    static LiteralSymbolPtr intern(const std::string &value);
    
    virtual bool isLiteralSymbol() const override;
    virtual bool isLiteralIdentifierSymbol() const override;
    virtual bool isAnonymousNameSymbol() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    
    virtual AnyValuePtr asUnarySelectorConvertedToIdentifier() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_SYMBOL_HPP