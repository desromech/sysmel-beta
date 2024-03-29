#ifndef SYSMEL_ENVIRONMENT_VARIANT_TYPE_HPP
#define SYSMEL_ENVIRONMENT_VARIANT_TYPE_HPP
#pragma once

#include "AggregateType.hpp"
#include <optional>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT VariantType : public SubMetaTypeOf<AggregateType, VariantType>
{
public:
    static TypePtr make(const TypePtrList &elementTypes);
    static VariantTypePtr makeNormalized(const TypePtrList &elementTypes);

    virtual bool isVariantType() const override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    
    virtual bool isNullableType() const override;

    virtual AggregateTypeValuePtr makeRawValueInstance() override;
    virtual AnyValuePtr basicNewValue() override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

    virtual std::string getQualifiedName() const override;
    virtual SExpression asSExpression() const override;

    virtual TypePtr appendTypeMakingVariant(const TypePtr &nextType) override;

    bool includesType(const TypePtr &typeToTest);

    void addSpecializedInstanceMethods();
    virtual ASTNodePtr analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    std::optional<uint64_t> findTypeSelectorIndexFor(const TypePtr &expecedType);
    virtual bool matchesExpectedValueTypeInPattern(const TypePtr &typeToMatch) override;

    VariantTypeValuePtr makeWithValueAndSelector(const AnyValuePtr &value, uint64_t selectorIndex);


    TypePtrList elementTypes;
    std::unordered_map<TypePtr, uint64_t> typeToSelectorMap;

protected:
    virtual void buildLayout() override;
};

/**
 * I am an instance of a function type object.
 */
class VariantTypeValue : public SubtypeOf<AggregateTypeValue, VariantTypeValue>
{
public:
    static constexpr char const __typeName__[] = "VariantType";
    static constexpr char const __sysmelTypeName__[] = "_VariantType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    static MethodCategories __instanceMacroMethods__();
    static MethodCategories __typeMacroMethods__();

    virtual bool isVariantTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    const AnyValuePtr &getTypeSelector() const;
    const AnyValuePtr &getWrappedElement() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRUCTURE_TYPE_HPP