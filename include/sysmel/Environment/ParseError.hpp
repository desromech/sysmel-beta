#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_PARSE_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_PARSE_ERROR_HPP
#pragma once

#include "CompilationError.hpp"
#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a collection of compilation errors.
 */
class ParseError : public SubtypeOf<CompilationError, ParseError>
{
public:
    static constexpr char const __typeName__[] = "ParseError";

    virtual std::string getDescription() const override;

    ASTParseErrorNodePtr parseErrorNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_PARSE_ERROR_HPP
