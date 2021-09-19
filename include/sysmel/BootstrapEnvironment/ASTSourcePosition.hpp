#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_POSITION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_POSITION_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class ASTSourcePosition;
typedef std::shared_ptr<ASTSourcePosition> ASTSourcePositionPtr;

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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_POSITION_HPP