#ifndef SYSMEL_ENVIRONMENT_AST_SOURCE_POSITION_HPP
#define SYSMEL_ENVIRONMENT_AST_SOURCE_POSITION_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourcePosition);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSourcePosition : public SubtypeOf<CompilerObject, ASTSourcePosition>
{
public:
    static constexpr char const __typeName__[] = "ASTSourcePosition";

    static ASTSourcePositionPtr empty();
    
    virtual bool isASTSourcePosition() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SOURCE_POSITION_HPP