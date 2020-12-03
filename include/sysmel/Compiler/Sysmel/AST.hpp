#ifndef SYSMEL_COMPILER_PARSER_AST_HPP
#define SYSMEL_COMPILER_PARSER_AST_HPP
#pragma once

#include "Token.hpp"
#include <memory>
#include <any>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

struct ASTVisitor;
struct ASTNode
{
    virtual std::any accept(ASTVisitor *visitor) = 0;

    SourcePosition sourcePosition;
};

typedef std::shared_ptr<ASTNode> ASTNodePtr;

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_AST_HPP
