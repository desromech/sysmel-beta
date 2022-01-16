#ifndef SYSMEL_ENVIRONMENT_META_TYPE_HPP
#define SYSMEL_ENVIRONMENT_META_TYPE_HPP
#pragma once

#include "Type.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class SYSMEL_COMPILER_LIB_EXPORT MetaType : public SubtypeOf<Type, MetaType>
{
public:
    static constexpr char const __typeName__[] = "MetaType";

    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual void setThisType(const TypePtr &instanceType);

    /// Is this type a meta type?.
    virtual bool isMetaType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

    /// This method returns the instance type.
    virtual TypePtr getInstanceType() override;

    /// This method returns the meta type.
    virtual TypePtr getMetaType() override;

    virtual std::string getValidNameString() const override;
    virtual std::string getValidNameStringIncludingTemplateName() const override;
    virtual std::string getQualifiedName() const override;
protected:
    TypePtr thisType;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_META_TYPE_HPP