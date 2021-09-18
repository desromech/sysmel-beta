#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_NODE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

class ASTSourcePosition;
typedef std::shared_ptr<ASTSourcePosition> ASTSourcePositionPtr;

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTNode : public SubtypeOf<CompilerObject, ASTNode>
{
public:
    static constexpr char const __typeName__[] = "ASTNode";

    virtual bool isASTNode() const override;

    /// This method encodes the AST node as a s-expression for debugging purposes.
    virtual AnyValuePtr encodeAsSExpression() const;

    virtual std::string printString() const;

    ASTSourcePositionPtr sourcePosition;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_NODE_HPP