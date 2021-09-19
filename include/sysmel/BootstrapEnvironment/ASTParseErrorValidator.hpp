#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_VALIDATOR_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_VALIDATOR_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTParseErrorValidator : public SubtypeOf<ASTVisitor, ASTParseErrorValidator>
{
public:
    static constexpr char const __typeName__[] = "ASTParseErrorValidator";

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP