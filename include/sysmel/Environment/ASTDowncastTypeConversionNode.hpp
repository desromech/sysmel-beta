#ifndef SYSMEL_ENVIRONMENT_AST_VALUE_DOWNCAST_TYPE_CONVERSION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_VALUE_DOWNCAST_TYPE_CONVERSION_NODE_HPP
#pragma once

#include "ASTTypeConversionNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTDowncastTypeConversionNode : public SubtypeOf<ASTTypeConversionNode, ASTDowncastTypeConversionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTDowncastTypeConversionNode";

    virtual bool isASTDowncastTypeConversionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_VALUE_DOWNCAST_TYPE_CONVERSION_NODE_HPP