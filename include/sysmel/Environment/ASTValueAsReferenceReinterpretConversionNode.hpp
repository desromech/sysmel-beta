#ifndef SYSMEL_ENVIRONMENT_AST_VALUE_AS_REFERENCE_REINTERPRET_TYPE_CONVERSION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_VALUE_AS_REFERENCE_REINTERPRET_TYPE_CONVERSION_NODE_HPP
#pragma once

#include "ASTTypeConversionNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class ASTValueAsReferenceReinterpretConversionNode : public SubtypeOf<ASTTypeConversionNode, ASTValueAsReferenceReinterpretConversionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTValueAsReferenceReinterpretConversionNode";

    virtual bool isASTValueAsReferenceReinterpretConversionNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_VALUE_AS_REFERENCE_REINTERPRET_TYPE_CONVERSION_NODE_HPP