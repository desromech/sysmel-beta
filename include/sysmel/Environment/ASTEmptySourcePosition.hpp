#ifndef SYSMEL_ENVIRONMENT_AST_EMPTY_SOURCE_POSITION_HPP
#define SYSMEL_ENVIRONMENT_AST_EMPTY_SOURCE_POSITION_HPP
#pragma once

#include "ASTSourcePosition.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTEmptySourcePosition : public SubtypeOf<ASTSourcePosition, ASTEmptySourcePosition>
{
public:
    static constexpr char const __typeName__[] = "ASTEmptySourcePosition";
    
    virtual bool isASTEmptySourcePosition() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_EMPTY_SOURCE_POSITION_HPP