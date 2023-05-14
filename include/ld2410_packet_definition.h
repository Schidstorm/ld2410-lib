#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <variant>
#include <functional>

#include "ld2410_packet_field.h"

#define I_COMMAND_ID(i) (i<<1)
#define I_COMMAND_ACK_ID(i) ((i<<1) | 1)

namespace ld2410 {

enum class PacketName: uint8_t {
    empty = 0,
    EnableConfigurationCommand = I_COMMAND_ID(1),
    EnableConfigurationCommandAck = I_COMMAND_ACK_ID(1),
    EndConfigurationCommand = I_COMMAND_ID(2),
    EndConfigurationCommandAck = I_COMMAND_ACK_ID(2),
    MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand = I_COMMAND_ID(3),
    MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck = I_COMMAND_ACK_ID(3),
    ReadParameterCommand = I_COMMAND_ID(4),
    ReadParameterCommandAck = I_COMMAND_ACK_ID(4),
    EnableEngineeringModeCommand = I_COMMAND_ID(5),
    EnableEngineeringModeCommandAck = I_COMMAND_ACK_ID(5),
    CloseEngineeringModeCommand = I_COMMAND_ID(6),
    CloseEngineeringModeCommandAck = I_COMMAND_ACK_ID(6),
    RangeSensitivityConfigurationCommand = I_COMMAND_ID(7),
    RangeSensitivityConfigurationCommandAck = I_COMMAND_ACK_ID(7),
    ReadFirmwareVersionCommand = I_COMMAND_ID(8),
    ReadFirmwareVersionCommandAck = I_COMMAND_ACK_ID(8),
    SetSerialPortBaudRate = I_COMMAND_ID(9),
    SetSerialPortBaudRateAck = I_COMMAND_ACK_ID(9),
    FactoryReset = I_COMMAND_ID(10),
    FactoryResetAck = I_COMMAND_ACK_ID(10),
    RestartModule = I_COMMAND_ID(11),
    RestartModuleAck = I_COMMAND_ACK_ID(11),
    ReportingDataFrame = I_COMMAND_ID(12),
    EngineeringModeDataFrame = I_COMMAND_ID(13)
};

constexpr PacketName packetNameAck(PacketName n) {
    union {
        PacketName n;
        uint8_t u8;
    } s = {};

    s.n = n;
    s.u8 |= 1;
    return s.n;
}

constexpr PacketName packetName(PacketName n) {
    union {
        PacketName n;
        uint8_t u8;
    } s = {};

    s.n = n;
    s.u8 &= ~1;
    return s.n;
}

std::ostream& operator<<(std::ostream& os, const PacketName name) {
    switch(name) {
        case PacketName::EnableConfigurationCommand: os << "EnableConfigurationCommand"; break;
        case PacketName::EnableConfigurationCommandAck: os << "EnableConfigurationCommandAck"; break;
        case PacketName::EndConfigurationCommand: os << "EndConfigurationCommand"; break;
        case PacketName::EndConfigurationCommandAck: os << "EndConfigurationCommandAck"; break;
        case PacketName::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand: os << "MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand"; break;
        case PacketName::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck: os << "MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck"; break;
        case PacketName::ReadParameterCommand: os << "ReadParameterCommand"; break;
        case PacketName::ReadParameterCommandAck: os << "ReadParameterCommandAck"; break;
        case PacketName::EnableEngineeringModeCommand: os << "EnableEngineeringModeCommand"; break;
        case PacketName::EnableEngineeringModeCommandAck: os << "EnableEngineeringModeCommandAck"; break;
        case PacketName::CloseEngineeringModeCommand: os << "CloseEngineeringModeCommand"; break;
        case PacketName::CloseEngineeringModeCommandAck: os << "CloseEngineeringModeCommandAck"; break;
        case PacketName::RangeSensitivityConfigurationCommand: os << "RangeSensitivityConfigurationCommand"; break;
        case PacketName::RangeSensitivityConfigurationCommandAck: os << "RangeSensitivityConfigurationCommandAck"; break;
        case PacketName::ReadFirmwareVersionCommand: os << "ReadFirmwareVersionCommand"; break;
        case PacketName::ReadFirmwareVersionCommandAck: os << "ReadFirmwareVersionCommandAck"; break;
        case PacketName::SetSerialPortBaudRate: os << "SetSerialPortBaudRate"; break;
        case PacketName::SetSerialPortBaudRateAck: os << "SetSerialPortBaudRateAck"; break;
        case PacketName::FactoryReset: os << "FactoryReset"; break;
        case PacketName::FactoryResetAck: os << "FactoryResetAck"; break;
        case PacketName::RestartModule: os << "RestartModule"; break;
        case PacketName::RestartModuleAck: os << "RestartModuleAck"; break;
        case PacketName::ReportingDataFrame: os << "ReportingDataFrame"; break;
        case PacketName::EngineeringModeDataFrame: os << "EngineeringModeDataFrame"; break;
        default: os << "<undefined>"; break;
    }
    return os;
}

struct PacketFieldDefinitionGeneratorOptionsSingle {
    uint16_t size;
};

struct PacketFieldDefinitionGeneratorOptionsMultiple {
    uint16_t size;
    FieldName count_field;
};

class PacketFieldDefinition {
private:
    FieldName m_name;
    //std::function<std::vector<PacketField>(std::function<uint32_t(FieldName)>)> m_field_generator;
    std::variant<PacketFieldDefinitionGeneratorOptionsSingle, PacketFieldDefinitionGeneratorOptionsMultiple> m_generator_options;

public:
    constexpr PacketFieldDefinition(FieldName name, uint16_t size): m_name(name), m_generator_options(PacketFieldDefinitionGeneratorOptionsSingle({size})) {

    }

    constexpr PacketFieldDefinition(FieldName name, FieldName multifield_count_name, uint16_t multifield_size): m_name(name), m_generator_options(PacketFieldDefinitionGeneratorOptionsMultiple({multifield_size, multifield_count_name}))  {

    }

    PacketFieldDefinition(): PacketFieldDefinition(FieldName::empty, 0) {

    }

    decltype(m_name) name() const {
        return m_name;
    }

    std::vector<PacketField> generate_fields(std::function<uint32_t(FieldName)> field_accessor) const {
        if (std::holds_alternative<PacketFieldDefinitionGeneratorOptionsSingle>(m_generator_options)) {
            auto opt = std::get<PacketFieldDefinitionGeneratorOptionsSingle>(m_generator_options);
            return { PacketField(m_name, opt.size) };
        } else {
            auto opt = std::get<PacketFieldDefinitionGeneratorOptionsMultiple>(m_generator_options);
            auto size = field_accessor(opt.count_field);
            std::vector<PacketField> fields;
            fields.reserve(size);
            for(uint16_t i = 0; i < size; i++) {
                fields.push_back(PacketField(m_name, opt.size));
            }
            return fields;
        }
    }



    // decltype(m_offset) offset() const {
    //     return m_offset;
    // }

    // void offset(decltype(m_offset) offset) {
    //     m_offset = offset;
    // }
};

class PacketDefinition {
private:
    PacketName m_name;
    uint32_t m_header;
    uint16_t m_type;
    uint32_t m_mfr;

    std::vector<PacketFieldDefinition> m_fieldDefinitions;

public:
    PacketDefinition(PacketName name, uint32_t header, uint16_t type, uint32_t mfr, std::vector<PacketFieldDefinition> &&fieldDefinitions): m_name(name), m_header(header), m_type(type), m_mfr(mfr), m_fieldDefinitions(std::move(fieldDefinitions)) {
        
    }

    PacketDefinition(): PacketDefinition(PacketName::empty, 0, 0, 0, {}) {

    }

    decltype(m_name) name() const {
        return m_name;
    }
    decltype(m_header) header() const {
        return m_header;
    }
    decltype(m_mfr) mfr() const {
        return m_mfr;
    }
    decltype(m_type) type() const {
        return m_type;
    }

    std::optional<size_t> find_field_definition(FieldName name) const {
        for(size_t i = 0; i < m_fieldDefinitions.size(); i++) {
            if (m_fieldDefinitions[i].name() == name) {
                return i;
            }
        }

        return std::nullopt;
    }

    const PacketFieldDefinition* get_field_definition(size_t index) const {
        return &m_fieldDefinitions[index];
    }

    std::vector<FieldName> get_names() const {
        std::vector<FieldName> names;
        names.resize(m_fieldDefinitions.size());

        for(size_t i = 0; i < m_fieldDefinitions.size(); i++) {
            names[i] = m_fieldDefinitions[i].name();
        }

        return names;
    }

    // uint16_t get_offset(size_t index) const {
    //     return m_fieldDefinitions[index].offset();
    // }

    

    // std::vector<FieldName> get_names() const {
    //     std::vector<FieldName> names;
    //     names.resize(m_fieldDefinitions.size());

    //     for(size_t i = 0; i < m_fieldDefinitions.size(); i++) {
    //         names[i] = m_fieldDefinitions[i].name();
    //     }

    //     return names;
    // }
};



const uint32_t CommandHeader = 0xfdfcfbfa;
const uint32_t CommandMFR = 0x04030201;
const uint32_t ReportingDataHeader = 0xf4f3f2f1;
const uint32_t ReportingDataMFR = 0x08070604;

const std::vector<PacketDefinition> definiedPackets = {
    { PacketName::EnableConfigurationCommand, CommandHeader, 0x00ff, CommandMFR, {
        { FieldName::value, 2}
    } },
    { PacketName::EnableConfigurationCommandAck, CommandHeader, 0x01ff, CommandMFR, {
        { FieldName::status, 2},
        { FieldName::protocol_version, 2},
        { FieldName::buffer, 2 }
    } },

    { PacketName::EndConfigurationCommand, CommandHeader, 0x00fe, CommandMFR, { } },
    { PacketName::EndConfigurationCommandAck, CommandHeader, 0x01ff, CommandMFR, {
        { FieldName::status, 2}
    } },

    
    { PacketName::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand, CommandHeader, 0x0060, CommandMFR, {
        { FieldName::maximumMovingDistanceWord, 2},
        { FieldName::maximumMovingDistanceParameter, 4},
        { FieldName::maximumStaticDistanceDoorWord, 2},
        { FieldName::maximumStaticDistanceDoorParameter, 4},
        { FieldName::noPersonDuration, 2},
        { FieldName::sectionUnattendedDuration, 4}
    } },
    { PacketName::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck, CommandHeader, 0x0160, CommandMFR, {
        { FieldName::status, 2}
    } },

    { PacketName::ReadParameterCommand, CommandHeader, 0x0061, CommandMFR, { } },
    { PacketName::ReadParameterCommandAck, CommandHeader, 0x0161, CommandMFR, {
        { FieldName::status, 2},
        { FieldName::header, 1},
        { FieldName::N, 1},
        {FieldName::distanceGateMotionSensitivity, FieldName::N, 1},
        {FieldName::distanceGateRestSensitivity, FieldName::N, 1},
        { FieldName::noTimeDuration, 2}
    } },

    { PacketName::EnableEngineeringModeCommand, CommandHeader, 0x0062, CommandMFR, {} },
    { PacketName::EnableEngineeringModeCommandAck, CommandHeader, 0x0162, CommandMFR, {
        { FieldName::status, 2}
    } },

    { PacketName::CloseEngineeringModeCommand, CommandHeader, 0x0063, CommandMFR, {} },
    { PacketName::CloseEngineeringModeCommandAck, CommandHeader, 0x0163, CommandMFR, {
        { FieldName::status, 2}
    } },


    //TODO: wtf is that documentation
    { PacketName::RangeSensitivityConfigurationCommand, CommandHeader, 0x0064, CommandMFR, {
        { FieldName::distanceGateWord, 2},
        { FieldName::distanceGateValue, 4},
        { FieldName::motionSensitivityWord, 2},
        { FieldName::motionSensitivityValue, 4},
        { FieldName::staticSensitivityWord, 2},
        { FieldName::staticSensitivityValue, 4}
    } },
    { PacketName::RangeSensitivityConfigurationCommandAck, CommandHeader, 0x0163, CommandMFR, {
        { FieldName::status, 2}
    } },

    { PacketName::ReadFirmwareVersionCommand, CommandHeader, 0x00a0, CommandMFR, {} },
    { PacketName::ReadFirmwareVersionCommandAck, CommandHeader, 0x01a0, CommandMFR, {
        { FieldName::status, 2},
        { FieldName::firmwareType, 2},
        { FieldName::majorVersionNumber, 2},
        { FieldName::minorVersionNumber, 4}
    } },

    { PacketName::SetSerialPortBaudRate, CommandHeader, 0x00a1, CommandMFR, {
        {FieldName::baudRateSelectionIndex, 2}
    } },
    { PacketName::SetSerialPortBaudRateAck, CommandHeader, 0x01a1, CommandMFR, {
        {FieldName::status, 2}
    } },

    { PacketName::FactoryReset, CommandHeader, 0x00a2, CommandMFR, {} },
    { PacketName::FactoryResetAck, CommandHeader, 0x01a2, CommandMFR, {
        {FieldName::status, 2}
    } },

    { PacketName::RestartModule, CommandHeader, 0x00a3, CommandMFR, {} },
    { PacketName::RestartModuleAck, CommandHeader, 0x01a3, CommandMFR, {
        {FieldName::status, 2}
    } },

    { PacketName::ReportingDataFrame,  ReportingDataHeader, 0xaa02, ReportingDataMFR, {
        {FieldName::targetState, 1},
        {FieldName::movementTargetDistance, 2},
        {FieldName::exerciseTargetEnergyValue, 1},
        {FieldName::stationaryTargetDistance, 2},
        {FieldName::stationaryTargetEnergyValue, 1},
        {FieldName::detectionDistance, 2},
        {FieldName::tail, 1},
        {FieldName::check, 1}
    }},

    { PacketName::EngineeringModeDataFrame,  ReportingDataHeader, 0xaa01, ReportingDataMFR, {
        {FieldName::targetState, 1},
        {FieldName::movementTargetDistance, 2},
        {FieldName::exerciseTargetEnergyValue, 1},
        {FieldName::stationaryTargetDistance, 2},
        {FieldName::stationaryTargetEnergyValue, 1},
        {FieldName::detectionDistance, 2},
        {FieldName::maximumMovingDistanceGateN, 1},
        {FieldName::maximumStaticDistanceGateN, 1},
        {FieldName::movementDistanceGateEnergyValue, FieldName::maximumMovingDistanceGateN, 1},
        {FieldName::staticDistanceGateEnergyValue, FieldName::maximumStaticDistanceGateN, 1},
    }}
};

}