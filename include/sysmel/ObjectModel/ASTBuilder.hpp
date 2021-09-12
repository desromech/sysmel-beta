#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_BUILDER_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_BUILDER_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am an utility for building AST nodes.
 */
class ASTBuilder : public SubtypeOf<CompilerObject, ASTBuilder>
{
public:
    static constexpr char const __typeName__[] = "ASTBuilder";

    virtual bool isASTBuilder() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_BUILDER_HPP