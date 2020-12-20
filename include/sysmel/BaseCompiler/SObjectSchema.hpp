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
struct SchemaEntityVisitor;
typedef std::shared_ptr<SchemaEntity> SchemaEntityPtr;

// I am a generic schema entity.
struct SchemaEntity
{
    virtual std::any accept(SchemaEntityVisitor &visitor) = 0;

    virtual bool isType() const { return false; }

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

    std::string superTypeName;
    DOM::Dictionary methods;
};

struct Schema
{
    std::vector<SchemaEntityPtr> entities;
    std::map<std::string, SchemaEntityPtr> entityNameDictionary;

    void addEntity(SchemaEntityPtr entity)
    {
        entities.push_back(entity);
        entityNameDictionary[entity->name] = entity;
    }
};
typedef std::shared_ptr<Schema> SchemaPtr;

SchemaPtr parseSchemaFromFileNamed(const std::string &fileName);

} // End of namespace SObjectSchema
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_BASE_COMPILER_SOBJECT_SCHEMA_HPP
