#ifndef SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP
#define SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP
#pragma once

#include "AggregateType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ArrayType : public SubMetaTypeOf<AggregateType, ArrayType>
{
public:
    static ArrayTypePtr make(const TypePtr &elementType, uint64_t size);
    
    virtual bool isArrayType() const override;
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
    
    virtual AggregateTypeValuePtr makeRawValueInstance() override;
    virtual AnyValuePtr basicNewValue() override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    void addSpecializedInstanceMethods();
    virtual ASTNodePtr analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    TypePtr elementType;
    uint64_t size;

protected:
    virtual void buildLayout() override;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT ArrayTypeValue : public SubtypeOf<AggregateTypeValue, ArrayTypeValue>
{
public:
    static constexpr char const __typeName__[] = "ArrayType";
    static constexpr char const __sysmelTypeName__[] = "_ArrayType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    static MethodCategories __typeMethods__();
    static MethodCategories __instanceMacroMethods__();

    virtual bool isArrayTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP