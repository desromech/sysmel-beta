#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_TYPE_HPP
#pragma once

#include "Type.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class MetaType : public SubtypeOf<Type, MetaType>
{
public:
    static constexpr char const __typeName__[] = "MetaType";

    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual void setThisType(const TypePtr &instanceType);

    /// Is this type a meta type?.
    virtual bool isMetaType() const override;

    /// This method returns the instance type.
    virtual TypePtr getInstanceType() override;

    /// This method returns the meta type.
    virtual TypePtr getMetaType() override;

    std::string printString() const override;
protected:
    TypePtr thisType;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_TYPE_HPP