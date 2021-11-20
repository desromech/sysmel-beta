#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_CONVERSION_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_CONVERSION_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTTypeConversionNode : public SubtypeOf<ASTNode, ASTTypeConversionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTypeConversionNode";

    virtual bool isASTTypeConversionNode() const override;

    ASTNodePtr expression;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_TYPE_CONVERSION_NODE_HPP