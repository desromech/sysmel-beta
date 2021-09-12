#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_CLOSURE_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_CLOSURE_NODE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTClosureNode : public SubtypeOf<CompilerObject, ASTClosureNode>
{
public:
    static constexpr char const __typeName__[] = "ASTClosureNode";

    virtual bool isASTClosureNode() const override;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_CLOSURE_NODE_HPP