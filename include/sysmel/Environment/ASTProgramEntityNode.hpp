#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PROGRAM_ENTITY_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PROGRAM_ENTITY_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);

/**
 * I am a node that is used for declaring or defining a program entity.
 */
class ASTProgramEntityNode : public SubtypeOf<ASTNode, ASTProgramEntityNode>
{
public:
    static constexpr char const __typeName__[] = "ASTProgramEntityNode";

    virtual bool isASTProgramEntityNode() const override;

    ProgramEntityPtr analyzedProgramEntity;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PROGRAM_ENTITY_NODE_HPP