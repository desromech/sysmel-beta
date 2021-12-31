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
class SYSMEL_COMPILER_LIB_EXPORT ASTSourcePosition : public SubtypeOf<CompilerObject, ASTSourcePosition>
{
public:
    static constexpr char const __typeName__[] = "ASTSourcePosition";

    static ASTSourcePositionPtr empty();
    
    static MethodCategories __instanceMethods__();

    virtual bool isASTSourcePosition() const override;

    virtual std::string getSourceCodeFileName() const;
    virtual std::string getSourceText() const;
    virtual int getLine() const;
    virtual int getColumn() const;
    virtual int getEndLine() const;
    virtual int getEndColumn() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SOURCE_POSITION_HPP