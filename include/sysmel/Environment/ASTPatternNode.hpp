#ifndef SYSMEL_ENVIRONMENT_AST_PATTERN_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_PATTERN_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTPatternNode : public SubtypeOf<ASTNode, ASTPatternNode>
{
public:
    static constexpr char const __typeName__[] = "ASTPatternNode";

    virtual bool isASTPatternNode() const override;

    virtual ASTNodePtr parseAsPatternNode() override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_PATTERN_NODE_HPP