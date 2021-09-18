#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_SOURCE_CODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_SOURCE_CODE_HPP
#pragma once

#include "ASTSourceCode.hpp"
#include "ASTSourcePosition.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSourceCodePosition : public SubtypeOf<ASTSourcePosition, ASTSourceCodePosition>
{
public:
    static constexpr char const __typeName__[] = "ASTSourceCodePosition";

    virtual bool isASTSourceCodePosition() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_SOURCE_CODE_HPP