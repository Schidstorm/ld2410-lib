#pragma once

#include <optional>
#include <vector>

#include "ld2410_packet_reader.h"
#include "ld2410_packet_writer.h"

namespace ld2410 {

using MaximumMovingDistanceGate = uint32_t;
using MaximumRestingDistanceDoor = uint32_t;
using SectionUnattendedDuration = uint32_t;

struct MaximumDistanceGateAndUnmannedDurationParameterConfigurationParameters {
    MaximumMovingDistanceGate maximum_moving_distance_gate;
    MaximumRestingDistanceDoor maximum_resting_distance_door;
    SectionUnattendedDuration section_unattended_duration;
};

struct ReadParameterResult {
    bool status;
    uint8_t n;
    std::vector<uint8_t> distance_gate_motion_sensitivity;
    std::vector<uint8_t> distance_gate_rest_sensitivity;
    uint16_t no_time_duration;
};


using DistanceGate = uint32_t;
using MotionSensitivity = uint32_t;
using StaticSensitivity = uint32_t;
struct RangeGateSensitivityConfigurationParameters {
    DistanceGate distance_gate;
    MotionSensitivity motion_sensitivity;
    StaticSensitivity static_sensitivity;
};


struct FirmwareVersion {
    bool status;
    uint16_t firmware_type;
    uint16_t major_version;
    uint32_t minior_version;
};

enum class SerialBaudRate: uint16_t {
    B9600 = 0x0001,
    B19200 = 0x0002,
    B38400 = 0x0003,
    B57600 = 0x0004,
    B115200 = 0x0005,
    B230400 = 0x0006,
    B256000 = 0x0007,
    B460800 = 0x0008,
};

class Commander {
private:
    PacketReader* m_packet_reader;
    PacketWriter* m_packet_writer;

public:
    Commander(PacketReader *packet_reader, PacketWriter *packet_writer): m_packet_reader(std::move(packet_reader)), m_packet_writer(std::move(packet_writer)) {

    }

    bool enable_configuration(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::EnableConfigurationCommand};
        packet.write(FieldName::value, 0x0001);
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    bool end_configuration(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::EndConfigurationCommand};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    bool set_maximum_distance_gateand_unmanned_duration_parameter_configuration(MaximumDistanceGateAndUnmannedDurationParameterConfigurationParameters p, unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand};
        packet.write(FieldName::maximumMovingDistanceWord, 0x0000);
        packet.write(FieldName::maximumMovingDistanceParameter, p.maximum_moving_distance_gate);
        packet.write(FieldName::maximumStaticDistanceDoorWord, 0x0001);
        packet.write(FieldName::maximumStaticDistanceDoorParameter, p.maximum_resting_distance_door);
        packet.write(FieldName::noPersonDuration, 0x0002);
        packet.write(FieldName::sectionUnattendedDuration, p.section_unattended_duration);
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    ReadParameterResult read_parameter(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::ReadParameterCommand};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            ReadParameterResult res;
            res.status = status == 0;
            res.n = resp->read_first_field(FieldName::N).value_or(0);
            res.distance_gate_motion_sensitivity = resp->read_all_fields<uint8_t>(FieldName::distanceGateMotionSensitivity);
            res.distance_gate_rest_sensitivity = resp->read_all_fields<uint8_t>(FieldName::distanceGateRestSensitivity);
            res.no_time_duration = resp->read_first_field(FieldName::noTimeDuration).value_or(0);
            return res;
        }
        return {
            .status = false
        };
    }

    bool enable_engineering_mode(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::EnableEngineeringModeCommand};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    bool close_engineering_mode(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::CloseEngineeringModeCommand};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    bool set_gate_sensitivity_configuration(RangeGateSensitivityConfigurationParameters p, unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::RangeSensitivityConfigurationCommand};
        packet.write(FieldName::distanceGateWord, 0x0000);
        packet.write(FieldName::distanceGateValue, p.distance_gate);
        packet.write(FieldName::motionSensitivityWord, 0x0001);
        packet.write(FieldName::motionSensitivityValue, p.motion_sensitivity);
        packet.write(FieldName::staticSensitivityWord, 0x0002);
        packet.write(FieldName::staticSensitivityValue, p.static_sensitivity);

        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    FirmwareVersion read_firmware_version(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::ReadFirmwareVersionCommand};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            FirmwareVersion res;
            res.status = status == 0;
            res.firmware_type = resp->read_first_field(FieldName::firmwareType).value_or(0);
            res.major_version = resp->read_first_field(FieldName::majorVersionNumber).value_or(0);
            res.minior_version = resp->read_first_field(FieldName::minorVersionNumber).value_or(0);
            return res;
        }
        return {
            .status = false
        };
    }

    bool set_serial_baud_rate(SerialBaudRate p, unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::SetSerialPortBaudRate};
        packet.write(FieldName::baudRateSelectionIndex, (uint16_t)p);

        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    bool factory_reset(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::FactoryReset};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }

    bool restart_module(unsigned long timeout_ms = default_timeout_ms) {
        Packet packet{PacketName::RestartModule};
        auto resp = m_packet_writer->write_and_wait_response(packet, *m_packet_reader, timeout_ms);
        if (resp.has_value()) {
            auto status = resp->read_first_field(FieldName::status);
            return status == 0;
        }
        return false;
    }
};

}