#ifndef SYSMEL_ENVIRONMENT_SEXPRESSION_HPP
#define SYSMEL_ENVIRONMENT_SEXPRESSION_HPP
#pragma once

#include "LargeInteger.hpp"
#include "Fraction.hpp"
#include <cstddef>
#include <variant>
#include <memory>

namespace Sysmel
{
namespace Environment
{

struct SExpressionIdentifier
{
    std::string value;
};

struct SExpressionSymbol
{
    std::string value;
};

struct SExpressionList;

struct SExpressionVoid {};

typedef std::variant<std::nullptr_t, bool, LargeInteger, Fraction, double, char32_t, SExpressionVoid, std::string, SExpressionIdentifier, SExpressionSymbol, SExpressionList> SExpression;

struct SExpressionList
{
    std::vector<SExpression> elements;
};

SYSMEL_COMPILER_LIB_EXPORT std::string sexpressionToString(const SExpression &sexpr);
SYSMEL_COMPILER_LIB_EXPORT std::string sexpressionToPrettyString(const SExpression &sexpr);
SYSMEL_COMPILER_LIB_EXPORT std::string sexpressionCanonicalizeString(const std::string &string);
SYSMEL_COMPILER_LIB_EXPORT SExpression stripSExpressionFromPositions(const SExpression &sexpr);

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SEXPRESSION_HPP