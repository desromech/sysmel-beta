#ifndef SYSMEL_BASE_COMPILER_SOBJECT_SCHEMA_HPP
#define SYSMEL_BASE_COMPILER_SOBJECT_SCHEMA_HPP
#pragma once

#include "DOM.hpp"
#include <any>

namespace SysmelMoebius
{
namespace Compiler
{
namespace SObjectSchema
{

struct SchemaEntity;
struct SchemaType;
struct SchemaMethod;

struct SchemaMethodExpression;

struct SchemaMethodSelfTypeExpression;
struct SchemaTypeToCTypeNameExpression;

struct SchemaLiteralValueMethodExpression;
struct SchemaVoidLiteralValue;
struct SchemaNilLiteralValue;
struct SchemaFalseLiteralValue;
struct SchemaTrueLiteralValue;
struct SchemaIntegerLiteralValue;
struct SchemaFloatLiteralValue;
struct SchemaCharacterLiteralValue;
struct SchemaStringLiteralValue;
struct SchemaSymbolLiteralValue;

struct SchemaEntityVisitor;
typedef std::shared_ptr<SchemaEntity> SchemaEntityPtr;
typedef std::shared_ptr<SchemaType> SchemaTypePtr;
typedef std::weak_ptr<SchemaType> SchemaTypeWeakPtr;
typedef std::shared_ptr<SchemaMethod> SchemaMethodPtr;
typedef std::shared_ptr<SchemaMethodExpression> SchemaMethodExpressionPtr;

// I am a generic schema entity.
struct SchemaEntity : std::enable_shared_from_this<SchemaEntity>
{
    virtual std::any accept(SchemaEntityVisitor &visitor) = 0;

    virtual bool isType() const { return false; }
    virtual bool isMethod() const { return false; }

    virtual bool isMethodExpression() const { return false; }

    virtual bool isSelfTypeExpression() const { return false; }
    virtual bool isTypeToCTypeNameExpression() const {return false; }

    virtual bool isLiteralValueMethodExpression() const { return false; }
    virtual bool isVoidLiteralValue() const { return false; }
    virtual bool isNilLiteralValue() const { return false; }
    virtual bool isFalseLiteralValue() const { return false; }
    virtual bool isTrueLiteralValue() const { return false; }
    virtual bool isIntegerLiteralValue() const { return false; }
    virtual bool isFloatLiteralValue() const { return false; }
    virtual bool isCharacterLiteralValue() const { return false; }
    virtual bool isStringLiteralValue() const { return false; }
    virtual bool isSymbolLiteralValue() const { return false; }

    std::string name;
    std::string fullNamespace;
    std::string sysmelName;
};

struct SchemaEntityVisitor
{
    virtual std::any visitEntity(SchemaEntity &entity)
    {
        return entity.accept(*this);
    }

    virtual std::any visitType(SchemaType &entity) = 0;
    virtual std::any visitMethod(SchemaMethod &method) = 0;

    virtual std::any visitSelfTypeExpression(SchemaMethodSelfTypeExpression &expression) = 0;
    virtual std::any visitTypeToCTypeNameExpression(SchemaTypeToCTypeNameExpression &expression) = 0;

    virtual std::any visitVoidLiteralValue(SchemaVoidLiteralValue &value) = 0;
    virtual std::any visitNilLiteralValue(SchemaNilLiteralValue &value) = 0;
    virtual std::any visitFalseLiteralValue(SchemaFalseLiteralValue &value) = 0;
    virtual std::any visitTrueLiteralValue(SchemaTrueLiteralValue &value) = 0;
    virtual std::any visitIntegerLiteralValue(SchemaIntegerLiteralValue &value) = 0;
    virtual std::any visitFloatLiteralValue(SchemaFloatLiteralValue &value) = 0;
    virtual std::any visitCharacterLiteralValue(SchemaCharacterLiteralValue &value) = 0;
    virtual std::any visitStringLiteralValue(SchemaStringLiteralValue &value) = 0;
    virtual std::any visitSymbolLiteralValue(SchemaSymbolLiteralValue &value) = 0;
};

struct SchemaType : SchemaEntity
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitType(*this);
    }

    virtual bool isType() const override
    {
        return true;
    }

    virtual SchemaMethodPtr lookupSelector(const std::string &selector) const
    {
        auto it = methodDictionary.find(selector);
        if(it != methodDictionary.end())
            return it->second;

        if(superType)
            return superType->lookupSelector(selector);

        return nullptr;
    }

    std::string superTypeName;
    std::map<std::string, SchemaMethodPtr> methodDictionary;

    SchemaTypePtr superType;
    std::string ctype;
    std::vector<SchemaMethodPtr> instanceDependentMethods;
};

struct SchemaMethod : SchemaEntity
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitMethod(*this);
    }

    virtual bool isMethod() const override
    {
        return true;
    }

    std::string selector;
    SchemaMethodExpressionPtr valueExpression;
    SchemaTypeWeakPtr returnType;

    bool isOverride = false;
};

struct SchemaMethodExpression : SchemaEntity
{
    virtual bool isMethodExpression() const override
    {
        return true;
    }

    virtual bool isInstanceTypeDependentExpression() const
    {
        return false;
    }
};

struct SchemaMethodSelfTypeExpression : SchemaMethodExpression
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitSelfTypeExpression(*this);
    }

    virtual bool isSelfTypeExpression() const override
    {
        return true;
    }

    virtual bool isInstanceTypeDependentExpression() const override
    {
        return true;
    }
};

struct SchemaTypeToCTypeNameExpression : SchemaMethodExpression
{
    SchemaMethodExpressionPtr typeExpression;

    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitTypeToCTypeNameExpression(*this);
    }

    virtual bool isTypeToCTypeNameExpression() const override
    {
        return true;
    }

    virtual bool isInstanceTypeDependentExpression() const override
    {
        return typeExpression->isInstanceTypeDependentExpression();
    }
};

struct SchemaLiteralValueMethodExpression : SchemaMethodExpression
{
    virtual bool isLiteralValueMethodExpression() const override
    {
        return true;
    }
};

struct SchemaVoidLiteralValue : SchemaLiteralValueMethodExpression
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitVoidLiteralValue(*this);
    }

    virtual bool isVoidLiteralValue() const override
    {
        return true;
    }
};

struct SchemaNilLiteralValue : SchemaLiteralValueMethodExpression
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitNilLiteralValue(*this);
    }

    virtual bool isNilLiteralValue() const override
    {
        return true;
    }
};

struct SchemaFalseLiteralValue : SchemaLiteralValueMethodExpression
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitFalseLiteralValue(*this);
    }

    virtual bool isFalseLiteralValue() const override
    {
        return true;
    }
};

struct SchemaTrueLiteralValue : SchemaLiteralValueMethodExpression
{
    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitTrueLiteralValue(*this);
    }

    virtual bool isTrueLiteralValue() const override
    {
        return true;
    }
};

struct SchemaIntegerLiteralValue : SchemaLiteralValueMethodExpression
{
    DOM::Integer value;

    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitIntegerLiteralValue(*this);
    }

    virtual bool isIntegerLiteralValue() const override
    {
        return true;
    }
};

struct SchemaFloatLiteralValue : SchemaLiteralValueMethodExpression
{
    DOM::Float value;

    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitFloatLiteralValue(*this);
    }

    virtual bool isFloatLiteralValue() const override
    {
        return true;
    }
};

struct SchemaCharacterLiteralValue : SchemaLiteralValueMethodExpression
{
    DOM::Character value;

    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitCharacterLiteralValue(*this);
    }

    virtual bool isCharacterLiteralValue() const override
    {
        return true;
    }
};

struct SchemaStringLiteralValue : SchemaLiteralValueMethodExpression
{
    DOM::String value;

    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitStringLiteralValue(*this);
    }

    virtual bool isStringLiteralValue() const override
    {
        return true;
    }
};

struct SchemaSymbolLiteralValue : SchemaLiteralValueMethodExpression
{
    DOM::Symbol value;

    virtual std::any accept(SchemaEntityVisitor &visitor) override
    {
        return visitor.visitSymbolLiteralValue(*this);
    }

    virtual bool isSymbolLiteralValue() const override
    {
        return true;
    }
};

struct Schema
{
    std::vector<SchemaEntityPtr> entities;
    std::map<std::string, SchemaEntityPtr> entityNameDictionary;

    std::vector<SchemaTypePtr> sortedTypes;

    void addEntity(SchemaEntityPtr entity)
    {
        entities.push_back(entity);
        entityNameDictionary[entity->name] = entity;
    }

    SchemaEntityPtr getEntityNamed(const std::string &name) const
    {
        auto it = entityNameDictionary.find(name);
        return it != entityNameDictionary.end() ? it->second : nullptr;
    }

    SchemaTypePtr getTypeNamed(const std::string &name) const
    {
        auto entity = getEntityNamed(name);
        if(!entity || !entity->isType())
            return nullptr;
        return std::static_pointer_cast<SchemaType> (entity);
    }

    SchemaTypePtr getRequiredTypeNamed(const std::string &name) const
    {
        auto result = getTypeNamed(name);
        if(!result)
            throw std::runtime_error("Failed to find required type named " + name);
        return result;
    }

};
typedef std::shared_ptr<Schema> SchemaPtr;

SchemaPtr parseSchemaFromFileNamed(const std::string &fileName);
void semanticAnalyzeSchema(const SchemaPtr &schema);

} // End of namespace SObjectSchema
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_BASE_COMPILER_SOBJECT_SCHEMA_HPP
