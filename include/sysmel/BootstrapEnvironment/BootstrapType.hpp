#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapType : public SubMetaTypeOf<SimpleType, BootstrapType>
{
public:
    static constexpr char const __typeName__[] = "BootstrapType";

    virtual bool isBootstrapType() const override;
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    virtual bool supportsMessageAnalysisByLiteralValueReceivers() const override;
    virtual bool isEphemeralCompileTimeObject() const override;
    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialCopyingFrom() override;
    virtual bool hasTrivialMovingFrom() override;

    void initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata);
    void addDefaultTypeConversionRules() override;

    virtual std::string printString() const override;
    
    virtual AnyValuePtr basicNewValue() override;
    virtual bool canBeInstantiatedWithLiteralValue(const AnyValuePtr &value) override;
    virtual AnyValuePtr instantiatedWithLiteralValue(const AnyValuePtr &value) override;
    virtual TypePtr asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment) override;
    
protected:
    const StaticBootstrapDefinedTypeMetadata *staticMetadata;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP