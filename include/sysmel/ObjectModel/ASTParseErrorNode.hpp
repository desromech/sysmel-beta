#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_PARSE_ERROR_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_PARSE_ERROR_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTParseErrorNode : public SubtypeOf<ASTNode, ASTParseErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTParseErrorNode";

    virtual bool isASTParseErrorNode() const override;

    std::string errorMessage;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_PARSE_ERROR_NODE_HPP