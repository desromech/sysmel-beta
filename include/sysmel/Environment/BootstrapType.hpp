#ifndef SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_HPP
#define SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapType : public SubMetaTypeOf<SimpleType, BootstrapType>
{
public:
    static constexpr char const __typeName__[] = "BootstrapType";

    virtual bool isBootstrapType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    virtual bool supportsMessageAnalysisByLiteralValueReceivers() const override;
    virtual bool isEphemeralCompileTimeObject() const override;
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

    void initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata);
    void addDefaultTypeConversionRules() override;

    virtual std::string printString() const override;
    
    virtual AnyValuePtr basicNewValue() override;
    virtual AnyValuePtr shallowCloneInstance(const AnyValuePtr &cloned) override;
    virtual bool canBeInstantiatedWithLiteralValue(const AnyValuePtr &value) override;
    virtual AnyValuePtr instantiatedWithLiteralValue(const AnyValuePtr &value) override;
    virtual TypePtr asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment) override;
    
protected:
    const StaticBootstrapDefinedTypeMetadata *staticMetadata;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_HPP