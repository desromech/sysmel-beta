#ifndef SYSMEL_OBJECT_MODEL_SOBJECT_SCHEMA_HPP
#define SYSMEL_OBJECT_MODEL_SOBJECT_SCHEMA_HPP
#pragma once

#include <stdint.h>
#include <string.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Object.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

inline size_t alignedTo(size_t offset, size_t alignment)
{
    return (offset + alignment - 1u) & (-alignment);
}
/**
 * I represent the type layout that corresponds to a specific type.
 */
enum SchemaTypeLayout : uint8_t
{
    Invalid = 0,

    // Basic type.
    Character = 'C',
    UnsignedInteger = 'u',
    SignedInteger = 's',
    Float = 'F',

    // Basic derived types.
    Alias = 'A',
    Enum = 'E',
    Pointer = 'P',

    // Aggregate types.
    GCObject = 'G',
    Structure = 'S',
    PackedStructure = 'p',
    Union = 'U',

    Void = 'V',
};

enum SchemaTypeFlags : uint32_t
{
    None = 0,
    IsMetaType = 1<<0,
};

/**
 * I am a type definition record.
 */
struct SchemaTypeDefinitionRecord
{
    SchemaTypeLayout layout;
    uint8_t reserved[3];

    uint32_t name;
    uint32_t flags;
    uint32_t metaType;
    uint32_t superType;
    uint32_t baseType;
    uint32_t instanceSize;
    uint32_t instanceAlignment;
    uint32_t slotTableOffset;
    uint32_t slotTableCount;
};

/**
 * I am a type definition record.
 */
struct SchemaTypeSlotDefinitionRecord
{
    uint32_t name;
    uint32_t type;
    uint32_t offset;
};

/**
 * I am a record for a string that is present in a schema.
 */
struct SchemaStringTableRecord
{
    uint32_t offset;
    uint32_t size;
};

/**
 * I am the header of a schema definition.
 *
 * Schema binary definition layout:
 * - Header
 * - Type definition table size.
 * - String table size.
 * - String data section. UTF-8 encoded without null padding.
 */
struct SchemaDefinitionHeader
{
    uint8_t magic[4]; // "SOSH"
    uint32_t schemaSize;
    uint32_t headerSize;
    uint32_t typeDefinitionTableSize;
    uint32_t typeSlotDefinitionTableSize;
    uint32_t stringTableSize;
    uint32_t stringDataSize;

    size_t typeDefinitionTableByteSize() const
    {
        return typeDefinitionTableSize * sizeof(SchemaTypeDefinitionRecord);
    }

    size_t typeSlotDefinitionTableByteSize() const
    {
        return typeSlotDefinitionTableSize * sizeof(SchemaTypeSlotDefinitionRecord);
    }

    size_t stringTableByteSize() const
    {
        return stringTableSize * sizeof(SchemaStringTableRecord);
    }

    bool hasValidMagicNumber() const
    {
        return memcmp(magic, "SOSH", 4) == 0
            && headerSize == sizeof(SchemaDefinitionHeader)
            && schemaSize >= sizeof(SchemaDefinitionHeader)
            && (headerSize + typeDefinitionTableByteSize() + typeSlotDefinitionTableByteSize() + stringTableByteSize() + stringDataSize) == schemaSize
            ;
    }

    void setValidMagicNumber()
    {
        memcpy(magic, "SOSH", 4);
        headerSize = sizeof(SchemaDefinitionHeader);
    }
};

class Schema;
typedef std::shared_ptr<Schema> SchemaPtr;

class SchemaTypeDefinition;
typedef std::shared_ptr<SchemaTypeDefinition> SchemaTypeDefinitionPtr;
typedef std::weak_ptr<SchemaTypeDefinition> SchemaTypeDefinitionWeakPtr;

struct SchemaTypeDefinitionSlot;
typedef std::shared_ptr<SchemaTypeDefinitionSlot> SchemaTypeDefinitionSlotPtr;

/**
 * I am a type slot definition.
 */
struct SchemaTypeDefinitionSlot : Object
{
    SYSMEL_CLASS_DECLARE(SchemaTypeDefinitionSlot, Object);

    std::string name;
    SchemaTypeDefinitionWeakPtr type;
    size_t offset = 0;
};

/**
 * I am an type definition.
 */
class SchemaTypeDefinition : public Object
{
public:
    SYSMEL_CLASS_DECLARE(SchemaTypeDefinition, Object);

    typedef std::vector<SchemaTypeDefinitionSlotPtr> Slots;

    SchemaTypeDefinitionPtr getMetaType() const;
    void setMetaType(const SchemaTypeDefinitionPtr &newMetaType);
    SchemaTypeDefinitionPtr getSuperType() const;
    void setSuperType(const SchemaTypeDefinitionPtr &newSuperType);

    void setLayoutNamed(const std::string &layoutName);
    void setSize(size_t value);
    void setAlignment(size_t value);

    bool isMetaType() const
    {
        return (flags & uint32_t(SchemaTypeFlags::IsMetaType)) != 0;
    }

    uint32_t index = 0;

    SchemaTypeLayout layout = SchemaTypeLayout::Invalid;
    uint32_t flags = 0;
    std::string name;
    SchemaTypeDefinitionWeakPtr metaType;
    SchemaTypeDefinitionWeakPtr superType;
    SchemaTypeDefinitionWeakPtr baseType;

    size_t instanceSize = 0;
    size_t instanceAlignment = 0;

    SchemaTypeDefinitionSlotPtr createSlot(const std::string &name, const SchemaTypeDefinitionPtr &type)
    {
        auto slot = std::make_shared<SchemaTypeDefinitionSlot> ();
        slot->name = name;
        slot->type = type;
        return addSlot(slot);
    }

    SchemaTypeDefinitionSlotPtr addSlot(const SchemaTypeDefinitionSlotPtr &slot)
    {
        slots.push_back(slot);
        if(!slot->name.empty())
            slotNameDictionary[slot->name] = slot;
        return slot;
    }

    SchemaTypeDefinitionPtr asConcreteType()
    {
        switch(layout)
        {
        case SchemaTypeLayout::Alias:
        case SchemaTypeLayout::Enum:
            return baseType.lock()->asConcreteType();
        default: return shared_from_this();
        }
    }

    void computeStructureLayout()
    {
        instanceSize = 0;
        instanceAlignment = 1;
        for (auto &slot : slots)
        {
            auto slotType = slot->type.lock()->asConcreteType();
            instanceSize = alignedTo(instanceSize, slotType->instanceAlignment);
            slot->offset = instanceSize;

            instanceSize += slotType->instanceSize;
            instanceAlignment = std::max(instanceAlignment, slotType->instanceAlignment);
        }

        instanceSize = alignedTo(instanceSize, instanceAlignment);
    }

    void computePackedStructureLayout()
    {
        instanceSize = 0;
        instanceAlignment = 1;
        for (auto &slot : slots)
        {
            auto slotType = slot->type.lock()->asConcreteType();
            slot->offset = instanceSize;

            instanceSize += slotType->instanceSize;
        }
    }

    void computeUnionLayout()
    {
        instanceSize = 0;
        instanceAlignment = 1;
        for (auto &slot : slots)
        {
            auto slotType = slot->type.lock()->asConcreteType();
            slot->offset = 0;
            instanceSize = std::max(instanceSize, slotType->instanceSize);
            instanceAlignment = std::max(instanceAlignment, slotType->instanceAlignment);
        }

        instanceSize = alignedTo(instanceSize, instanceAlignment);
    }

    void computeSlotLayout()
    {
        switch(layout)
        {
        case SchemaTypeLayout::Structure: return computeStructureLayout();
        case SchemaTypeLayout::PackedStructure: return computePackedStructureLayout();
        case SchemaTypeLayout::Union: return computeUnionLayout();
        default: abort();
        }
    }

    const Slots &getSlots() const
    {
        return slots;
    }

    SchemaTypeDefinitionSlotPtr getSlotNamed(const std::string &name) const
    {
        auto it = slotNameDictionary.find(name);
        return it != slotNameDictionary.end() ? it->second : nullptr;
    }

private:
    Slots slots;
    std::unordered_map<std::string, SchemaTypeDefinitionSlotPtr> slotNameDictionary;
};

/**
 * I am an schema for the object layouts.
 */
class Schema : public Object
{
public:
    SYSMEL_CLASS_DECLARE(Schema, Object);
    typedef std::vector<SchemaTypeDefinitionPtr> TypeDefinitions;

    SchemaTypeDefinitionPtr addTypeDefinition(const SchemaTypeDefinitionPtr &typeDefinition)
    {
        typeDefinitions.push_back(typeDefinition);
        typeDefinition->index = typeDefinitions.size();
        if(!typeDefinition->name.empty())
            typeDefinitionMap[typeDefinition->name] = typeDefinition;
        return typeDefinition;
    }

    const TypeDefinitions &getTypeDefinitions() const
    {
        return typeDefinitions;
    }

    SchemaTypeDefinitionPtr getTypeDefinitionNamed(const std::string &name) const
    {
        auto it = typeDefinitionMap.find(name);
        return it != typeDefinitionMap.end() ? it->second : nullptr;
    }

    std::vector<uint8_t> serializeToBinaryData() const;
    static SchemaPtr deserializeFromBinaryData(const std::vector<uint8_t> &data);

    SchemaTypeDefinitionPtr createBasicType(const std::string name, SchemaTypeLayout layout, size_t instanceSize, size_t instanceAlignment)
    {
        auto type = std::make_shared<SchemaTypeDefinition> ();
        type->layout = layout;
        type->name = name;
        type->instanceSize = instanceSize;
        type->instanceAlignment = instanceAlignment;
        return addTypeDefinition(type);
    }

    SchemaTypeDefinitionPtr createAggregateType(const std::string name, SchemaTypeLayout layout)
    {
        return createBasicType(name, layout, 0, 1);
    }

    SchemaTypeDefinitionPtr createStructureType(const std::string name)
    {
        return createAggregateType(name, SchemaTypeLayout::Structure);
    }

    SchemaTypeDefinitionPtr createPackedStructureType(const std::string name)
    {
        return createAggregateType(name, SchemaTypeLayout::PackedStructure);
    }

    SchemaTypeDefinitionPtr createUnionType(const std::string name)
    {
        return createAggregateType(name, SchemaTypeLayout::Union);
    }

    SchemaTypeDefinitionPtr createDerivedType(const std::string name, SchemaTypeLayout layout, const SchemaTypeDefinitionPtr &baseType, size_t instanceSize, size_t instanceAlignment)
    {
        auto type = std::make_shared<SchemaTypeDefinition> ();
        type->layout = layout;
        type->name = name;
        type->baseType = baseType;
        type->instanceSize = instanceSize;
        type->instanceAlignment = instanceAlignment;
        return addTypeDefinition(type);
    }

    SchemaTypeDefinitionPtr createAliasType(const std::string name, const SchemaTypeDefinitionPtr &baseType)
    {
        return createDerivedType(name, SchemaTypeLayout::Alias, baseType, 0, 0);
    }

private:
    TypeDefinitions typeDefinitions;
    std::unordered_map<std::string, SchemaTypeDefinitionPtr> typeDefinitionMap;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_SOBJECT_SCHEMA_HPP
