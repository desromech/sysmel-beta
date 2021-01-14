#include "sysmel/ObjectModel/SObjectSchema.hpp"
#include <iostream>

namespace SysmelMoebius
{
namespace ObjectModel
{

//==============================================================================
// SchemaTypeDefinition
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(SchemaTypeDefinitionSlot)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(SchemaTypeDefinitionSlot)
END_SYSMEL_CLASS_SIDE_METHODS()

//==============================================================================
// SchemaTypeDefinition
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(SchemaTypeDefinition)
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getMetaType, "type"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(setMetaType, "type:"),

    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getSuperType, "supertype"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(setSuperType, "supertype:"),

    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(setLayoutNamed, "layoutNamed:"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(setSize, "size:"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(setAlignment, "alignment:"),
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(SchemaTypeDefinition)
END_SYSMEL_CLASS_SIDE_METHODS()

SchemaTypeDefinitionPtr SchemaTypeDefinition::getMetaType() const
{
    return metaType.lock();
}

void SchemaTypeDefinition::setMetaType(const SchemaTypeDefinitionPtr &newMetaType)
{
    metaType = newMetaType;
}

SchemaTypeDefinitionPtr SchemaTypeDefinition::getSuperType() const
{
    return superType.lock();
};

void SchemaTypeDefinition::setSuperType(const SchemaTypeDefinitionPtr &newSuperType)
{
    superType = newSuperType;
    if(!newSuperType)
        return;
        
    if(!isMetaType())
    {
        auto metaTypeValue = metaType.lock();
        auto superMetaType = newSuperType->metaType.lock();
        if(metaTypeValue)
        {
            metaTypeValue->superType = superMetaType;
            if(!metaTypeValue->metaType.lock())
                metaTypeValue->metaType = superMetaType->metaType.lock();
        }
    }
}

static std::unordered_map<std::string, SchemaTypeLayout> layoutNameMap = {
    {"Invalid", SchemaTypeLayout::Invalid},
    {"Character", SchemaTypeLayout::Character},
    {"UnsignedInteger", SchemaTypeLayout::UnsignedInteger},
    {"SignedInteger", SchemaTypeLayout::SignedInteger},
    {"Float", SchemaTypeLayout::Float},
    {"Alias", SchemaTypeLayout::Alias},
    {"Enum", SchemaTypeLayout::Enum},
    {"Pointer", SchemaTypeLayout::Pointer},
    {"GCObject", SchemaTypeLayout::GCObject},
    {"Structure", SchemaTypeLayout::Structure},
    {"PackedStructure", SchemaTypeLayout::PackedStructure},
    {"Union", SchemaTypeLayout::Union},
    {"Void", SchemaTypeLayout::Void},
};

void SchemaTypeDefinition::setLayoutNamed(const std::string &layoutName)
{
    auto it = layoutNameMap.find(layoutName);
    if(it == layoutNameMap.end())
        throw std::runtime_error("Unsupported schema type layout named " + layoutName + ".");
    layout = it->second;
}

void SchemaTypeDefinition::setSize(size_t value)
{
    instanceSize = value;
}
void SchemaTypeDefinition::setAlignment(size_t value)
{
    instanceAlignment = value;
}


//==============================================================================
// SchemaTypeDefinition
//==============================================================================


BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(Schema)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(Schema)
END_SYSMEL_CLASS_SIDE_METHODS()

struct SchemaStringDataWriter
{
    uint32_t internString(const std::string &string)
    {
        if(string.empty())
            return 0;

        auto it = writtenStrings.find(string);
        if(it != writtenStrings.end())
            return it->second;

        SchemaStringTableRecord record{uint32_t(dataSection.size()), uint32_t(string.size())};
        records.push_back(record);
        dataSection.insert(dataSection.end(), string.begin(), string.end());
        return writtenStrings[string] = uint32_t(records.size());
    }

    size_t encodedSize() const
    {
        return records.size() * sizeof(SchemaStringTableRecord) + dataSection.size();
    }

    std::vector<SchemaStringTableRecord> records;
    std::vector<char> dataSection;
    std::unordered_map<std::string, uint32_t> writtenStrings;
};

std::vector<uint8_t> Schema::serializeToBinaryData() const
{
    SchemaStringDataWriter stringWriter;
    std::vector<SchemaTypeDefinitionRecord> typeDefinitionTable;
    std::vector<SchemaTypeSlotDefinitionRecord> typeSlotDefinitionTable;
    typeDefinitionTable.reserve(typeDefinitions.size());

    for(auto &typeDefinition : typeDefinitions)
    {
        SchemaTypeDefinitionRecord record{};
        record.layout = typeDefinition->layout;
        record.flags = typeDefinition->flags;
        record.name = stringWriter.internString(typeDefinition->name);
        record.slotTableOffset = typeSlotDefinitionTable.size();
        record.instanceSize = uint32_t(typeDefinition->instanceSize);
        record.instanceAlignment = uint32_t(typeDefinition->instanceAlignment);
        auto metaType = typeDefinition->metaType.lock();
        record.metaType = metaType ? metaType->index : 0;
        auto baseType = typeDefinition->baseType.lock();
        record.baseType = baseType ? baseType->index : 0;
        auto superType = typeDefinition->superType.lock();
        record.superType = superType ? superType->index : 0;

        auto &slots = typeDefinition->getSlots();
        record.slotTableCount = slots.size();
        for(auto &slot : slots)
        {
            SchemaTypeSlotDefinitionRecord slotRecord{};
            slotRecord.type = slot->type.lock()->index;
            slotRecord.name = stringWriter.internString(slot->name);
            slotRecord.offset = uint32_t(slot->offset);
            typeSlotDefinitionTable.push_back(slotRecord);
        }

        typeDefinitionTable.push_back(record);
    }

    auto typeDefinitionTableSize = typeDefinitionTable.size() * sizeof(SchemaTypeDefinitionRecord);
    auto typeSlotDefinitionTableSize = typeSlotDefinitionTable.size() * sizeof(SchemaTypeSlotDefinitionRecord);
    SchemaDefinitionHeader header{};
    header.setValidMagicNumber();
    header.schemaSize = uint32_t(sizeof(SchemaDefinitionHeader) + typeDefinitionTableSize + typeSlotDefinitionTableSize + stringWriter.encodedSize());
    header.typeDefinitionTableSize = uint32_t(typeDefinitionTable.size());
    header.typeSlotDefinitionTableSize = uint32_t(typeSlotDefinitionTable.size());
    header.stringTableSize = stringWriter.records.size();
    header.stringDataSize = uint32_t(stringWriter.dataSection.size());

    std::vector<uint8_t> serializedData(header.schemaSize);
    auto destPointer = &serializedData[0];

    memcpy(destPointer, &header, sizeof(SchemaDefinitionHeader));
    destPointer += sizeof(SchemaDefinitionHeader);

    if(!typeDefinitionTable.empty())
    {
        memcpy(destPointer, &typeDefinitionTable[0], typeDefinitionTableSize);
        destPointer += typeDefinitionTableSize;
    }

    if(!typeSlotDefinitionTable.empty())
    {
        memcpy(destPointer, &typeSlotDefinitionTable[0], typeSlotDefinitionTableSize);
        destPointer += typeSlotDefinitionTableSize;
    }

    if(!stringWriter.records.empty())
    {
        memcpy(destPointer, &stringWriter.records[0], stringWriter.records.size() * sizeof(SchemaStringTableRecord));
        destPointer += stringWriter.records.size() * sizeof(SchemaStringTableRecord);

        memcpy(destPointer, &stringWriter.dataSection[0], stringWriter.dataSection.size());
        destPointer += stringWriter.dataSection.size();
    }

    return serializedData;
}

SchemaPtr Schema::deserializeFromBinaryData(const std::vector<uint8_t> &data)
{
    if(data.size() < sizeof(SchemaDefinitionHeader))
        return nullptr;

    auto sourcePointer = &data[0];
    SchemaDefinitionHeader header{};
    memcpy(&header, sourcePointer, sizeof(SchemaDefinitionHeader));
    sourcePointer += sizeof(SchemaDefinitionHeader);

    if(!header.hasValidMagicNumber() || data.size() != header.schemaSize)
        return nullptr;

    // Read the type definition table.
    std::vector<SchemaTypeDefinitionRecord> typeDefinitionTable(header.typeDefinitionTableSize);
    if(!typeDefinitionTable.empty())
    {
        memcpy(&typeDefinitionTable[0], sourcePointer, header.typeDefinitionTableByteSize());
        sourcePointer += header.typeDefinitionTableByteSize();
    }

    // Read the type slot definition table.
    std::vector<SchemaTypeSlotDefinitionRecord> typeSlotDefinitionTable(header.typeSlotDefinitionTableSize);
    if(!typeSlotDefinitionTable.empty())
    {
        memcpy(&typeSlotDefinitionTable[0], sourcePointer, header.typeSlotDefinitionTableByteSize());
        sourcePointer += header.typeSlotDefinitionTableByteSize();
    }

    // Read the strings.
    std::vector<SchemaStringTableRecord> stringRecords(header.stringTableSize);
    std::vector<char> stringDataSection(header.stringDataSize);
    if(!stringRecords.empty())
    {
        memcpy(&stringRecords[0], sourcePointer, header.stringTableByteSize());
        sourcePointer += header.stringTableByteSize();
    }

    if(!stringDataSection.empty())
    {
        memcpy(&stringDataSection[0], sourcePointer, header.stringDataSize);
        sourcePointer += header.stringDataSize;
    }

    // Decode the strings.
    std::vector<std::string> decodedStrings;
    decodedStrings.reserve(stringRecords.size());
    for(auto &record : stringRecords)
    {
        // Validate the range.
        if(record.offset > stringDataSection.size() || record.offset + record.size > stringDataSection.size())
            return nullptr;

        // Read the string content.
        decodedStrings.push_back(std::string(stringDataSection.begin() + record.offset, stringDataSection.begin() + (record.offset + record.size)));
    }

    // Allocate the types.
    std::vector<SchemaTypeDefinitionPtr> types;
    types.reserve(typeDefinitionTable.size());
    for(size_t i = 0; i < typeDefinitionTable.size(); ++i)
        types.push_back(std::make_shared<SchemaTypeDefinition> ());

    // Decode the type slots.
    std::vector<SchemaTypeDefinitionSlotPtr> typeSlots;
    typeSlots.reserve(typeSlotDefinitionTable.size());
    for(auto &record : typeSlotDefinitionTable)
    {
        auto slot = std::make_shared<SchemaTypeDefinitionSlot> ();

        // Read the slot name.
        if(record.name != 0)
        {
            if(record.name > decodedStrings.size())
                return nullptr;
            slot->name = decodedStrings[record.name - 1];
        }

        if(record.type != 0)
        {
            if(record.type > types.size())
                return nullptr;
            slot->type = types[record.type - 1];
        }

        slot->offset = record.offset;
        typeSlots.push_back(slot);
    }

    // Decode the types.
    auto schema = std::make_shared<Schema> ();
    for(size_t i = 0; i < typeDefinitionTable.size(); ++i)
    {
        auto &record = typeDefinitionTable[i];
        auto &type = types[i];

        // Read the type name, if present.
        if(record.name != 0)
        {
            if(record.name > decodedStrings.size())
                return nullptr;
            type->name = decodedStrings[record.name - 1];
        }

        if(record.metaType != 0)
        {
            if(record.metaType > types.size())
                return nullptr;
            type->metaType = types[record.metaType - 1];
        }

        if(record.superType != 0)
        {
            if(record.superType > types.size())
                return nullptr;
            type->superType = types[record.superType - 1];
        }

        if(record.baseType != 0)
        {
            if(record.baseType > types.size())
                return nullptr;
            type->baseType = types[record.baseType - 1];
        }

        type->layout = record.layout;
        type->flags = record.flags;
        type->instanceSize = record.instanceSize;
        type->instanceAlignment = record.instanceAlignment;

        if(record.slotTableOffset > typeSlots.size() || record.slotTableOffset + record.slotTableCount > typeSlots.size())
            return nullptr;

        for(size_t i = 0; i < record.slotTableCount; ++i)
            type->addSlot(typeSlots[record.slotTableOffset + i]);

        schema->addTypeDefinition(type);
    }

    return schema;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
