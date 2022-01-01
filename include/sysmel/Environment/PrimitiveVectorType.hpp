#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_VECTOR_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_VECTOR_TYPE_HPP
#pragma once

#include "PrimitiveTensorType.hpp"
#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveVectorType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveVectorTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT PrimitiveVectorType : public SubMetaTypeOf<SimpleType, PrimitiveVectorType>
{
public:
    static TypePtr make(const TypePtr &elementType, uint32_t elements);

    virtual PrimitiveVectorTypeValuePtr withValues(const AnyValuePtrList &elements);
    virtual PrimitiveVectorTypeValuePtr withAll(const AnyValuePtr &element);
    
    virtual bool isPrimitiveVectorType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    
    virtual bool isNullableType() const override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;
    
    virtual AnyValuePtr basicNewValue() override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    virtual ASTNodePtr analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    void addSpecializedInstanceMethods();

    TypePtr elementType;
    uint32_t elements;
};

/**
 * I am a single scalar primitive type.
 */
class SYSMEL_COMPILER_LIB_EXPORT PrimitiveVectorTypeValue : public SubtypeOf<PrimitiveTensorType, PrimitiveVectorTypeValue>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveVectorType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveVectorType";

    virtual bool isPrimitiveVectorTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    AnyValuePtr reduce(const AnyValuePtr &selector);
    AnyValuePtr reduce(const std::string &selector);

    PrimitiveVectorTypePtr type;
    AnyValuePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_VECTOR_TYPE_HPP