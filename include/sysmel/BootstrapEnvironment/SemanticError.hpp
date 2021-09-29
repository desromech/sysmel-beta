#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_PARSE_ERROR_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_PARSE_ERROR_HPP
#pragma once

#include "CompilationError.hpp"
#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a collection of compilation errors.
 */
class SemanticError : public SubtypeOf<CompilationError, SemanticError>
{
public:
    static constexpr char const __typeName__[] = "SemanticError";

    virtual std::string getDescription() const override;

    ASTSemanticErrorNodePtr semanticErrorNode;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_PARSE_ERROR_HPP
