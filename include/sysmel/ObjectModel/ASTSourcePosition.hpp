#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_SOURCE_POSITION_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_SOURCE_POSITION_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSourcePosition : public SubtypeOf<CompilerObject, ASTSourcePosition>
{
public:
    static constexpr char const __typeName__[] = "ASTSourcePosition";

    virtual bool isASTSourcePosition() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_SOURCE_POSITION_HPP