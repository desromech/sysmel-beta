#ifndef SYSMEL_ENVIRONMENT_DECORATED_TYPE_HPP
#define SYSMEL_ENVIRONMENT_DECORATED_TYPE_HPP
#pragma once

#include "DerivedType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(DecoratedType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DecoratedTypeValue);

/**
 * I am an instance of a function type object.
 */
class DecoratedType : public SubMetaTypeOf<DerivedType, DecoratedType>
{
public:
    static TypePtr make(const TypePtr &baseType, TypeDecorationFlags decorations);

    virtual bool isDecoratedType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

    virtual bool isConstDecoratedType() const override;
    virtual bool isVolatileDecoratedType() const override;
    virtual bool isRestrictDecoratedType() const override;

    virtual bool supportsDynamicCompileTimeMessageSend() const override;

    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialCopyingFrom() override;
    virtual bool hasTrivialMovingFrom() override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;
    
    virtual TypePtr asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment) override;
    virtual TypePtr withDecorations(TypeDecorationFlags decorations) override;
    virtual TypePtr asUndecoratedType() override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    void addSpecializedInstanceMethods();

    DecoratedTypeValuePtr makeWithValue(const AnyValuePtr &value);

    TypeDecorationFlags decorations = TypeDecorationFlags::None;
};

/**
 * I am an instance of a function type object.
 */
class DecoratedTypeValue : public SubtypeOf<DerivedTypeValue, DecoratedTypeValue>
{
public:
    static constexpr char const __typeName__[] = "DecoratedType";
    static constexpr char const __sysmelTypeName__[] = "_DecoratedType";

    virtual bool isDecoratedTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_DECORATED_TYPE_HPP