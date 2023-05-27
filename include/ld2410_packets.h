#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <variant>

#include "ld2410_reader.h"
#include "ld2410_writer.h"


#define LD2410_GETTER(x) decltype(m_##x) x() const { return m_##x; }
#define LD2410_SETTER(x) void x(decltype(m_##x) v) { m_##x = v; }

#define LD2410_PROP(t, x) private:\
t m_##x; \
public: \
LD2410_GETTER(x) \
LD2410_SETTER(x)

#define LD2410_READ_SHORT(x) x(read_any<decltype(m_##x)>(reader))

#define LD2410_WRITE_SHORT(x) write_any<decltype(m_##x)>(writer, x())
#define LD2410_WRITE_SWAPPED_SHORT(x) x(write_any_swapped<decltype(m_##x)>(writer))

namespace ld2410 {
    const uint32_t CommandHeader = 0xfafbfcfd;
    const uint32_t CommandMFR = 0x01020304;
    const uint32_t ReportingDataHeader = 0xf1f2f3f4;
    const uint32_t ReportingDataMFR = 0x04060708;

    template<typename T>
    class to_bytes_union {
    public:
        union val_t {
            T val;
            uint8_t u8[sizeof(T)];
            constexpr val_t(T v): val(v) {

            }
        } val;

        constexpr explicit to_bytes_union(T v): val(v) {
            
        }
    };


    class EngineeringModeDataFrame {
        LD2410_PROP(uint8_t, target_state)
        LD2410_PROP(uint16_t, movement_target_distance)
        LD2410_PROP(uint8_t, exercise_target_energy_value)
        LD2410_PROP(uint16_t, stationary_target_distance)
        LD2410_PROP(uint8_t, stationary_target_energy_value)
        LD2410_PROP(uint16_t, detection_distance)
        LD2410_PROP(uint8_t, maximum_moving_distance_gate_n)
        LD2410_PROP(uint8_t, maximum_static_distance_gate_n)
        LD2410_PROP(std::vector<uint8_t>, movement_distance_gate_energy_value)
        LD2410_PROP(std::vector<uint8_t>, static_distance_gate_energy_value)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{ReportingDataHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{ReportingDataMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0xaa01};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(target_state);
            LD2410_READ_SHORT(movement_target_distance);
            LD2410_READ_SHORT(exercise_target_energy_value);
            LD2410_READ_SHORT(stationary_target_distance);
            LD2410_READ_SHORT(stationary_target_energy_value);
            LD2410_READ_SHORT(detection_distance);
            LD2410_READ_SHORT(maximum_moving_distance_gate_n);
            LD2410_READ_SHORT(maximum_static_distance_gate_n);

            std::vector<uint8_t> b;
            b.resize(maximum_moving_distance_gate_n());
            for(size_t i = 0; i < b.size(); ++i) {
                b[i] = reader();
            }
            movement_distance_gate_energy_value(b);

            b.resize((size_t)maximum_static_distance_gate_n());
            for(size_t i = 0; i < b.size(); ++i) {
                b[i] = reader();
            }
            static_distance_gate_energy_value(b);
        }
    };

    class ReportingDataFrame {
        LD2410_PROP(uint8_t, target_state)
        LD2410_PROP(uint16_t, movement_target_distance)
        LD2410_PROP(uint8_t, exercise_target_energy_value)
        LD2410_PROP(uint16_t, stationary_target_distance)
        LD2410_PROP(uint8_t, stationary_target_energy_value)
        LD2410_PROP(uint16_t, detection_distance)
        LD2410_PROP(uint8_t, tail)
        LD2410_PROP(uint8_t, check)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{ReportingDataHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{ReportingDataMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0xaa02};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(target_state);
            LD2410_READ_SHORT(movement_target_distance);
            LD2410_READ_SHORT(exercise_target_energy_value);
            LD2410_READ_SHORT(stationary_target_distance);
            LD2410_READ_SHORT(stationary_target_energy_value);
            LD2410_READ_SHORT(detection_distance);
            LD2410_READ_SHORT(tail);
            LD2410_READ_SHORT(check);
        }
    };

    class EnableConfigurationCommandAck {
        LD2410_PROP(uint16_t, status)
        LD2410_PROP(uint16_t, protocol_version)
        LD2410_PROP(uint16_t, buffer)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x01ff};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
            LD2410_READ_SHORT(protocol_version);
            LD2410_READ_SHORT(buffer);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(EnableConfigurationCommandAck::m_status);
            size_ += sizeof(EnableConfigurationCommandAck::m_protocol_version);
            size_ += sizeof(EnableConfigurationCommandAck::m_buffer);
            return size_;
        }
    };

    class EnableConfigurationCommand {
        LD2410_PROP(uint16_t, value)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x00ff};
        using ack_t = EnableConfigurationCommandAck;

        void write(const writer_t &writer) const {
            LD2410_WRITE_SHORT(value);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(EnableConfigurationCommand::m_value);
            return size_;
        }
    };

    class EndConfigurationCommandAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x01fe};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(EndConfigurationCommandAck::m_status);
            return size_;
        }
    };

    class EndConfigurationCommand {

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x00fe};
        using ack_t = EndConfigurationCommandAck;

        void write(const writer_t &writer) const {
            
        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };

    class MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0160};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck::m_status);
            return size_;
        }
    };

    class MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand {
        LD2410_PROP(uint16_t, maximum_moving_distance_word)
        LD2410_PROP(uint32_t, maximum_moving_distance_parameter)
        LD2410_PROP(uint16_t, maximum_static_distance_door_word)
        LD2410_PROP(uint32_t, maximum_static_distance_door_parameter)
        LD2410_PROP(uint16_t, no_person_duration)
        LD2410_PROP(uint32_t, section_unattended_duration)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0060};
        using ack_t = MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck;

        void write(const writer_t &writer) const {
            LD2410_WRITE_SHORT(maximum_moving_distance_word);
            LD2410_WRITE_SHORT(maximum_moving_distance_parameter);
            LD2410_WRITE_SHORT(maximum_static_distance_door_word);
            LD2410_WRITE_SHORT(maximum_static_distance_door_parameter);
            LD2410_WRITE_SHORT(no_person_duration);
            LD2410_WRITE_SHORT(section_unattended_duration);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand::m_maximum_moving_distance_word);
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand::m_maximum_moving_distance_parameter);
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand::m_maximum_static_distance_door_word);
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand::m_maximum_static_distance_door_parameter);
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand::m_no_person_duration);
            size_ += sizeof(MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand::m_section_unattended_duration);
            return size_;
        }
    };

    class ReadParameterCommandAck {
        LD2410_PROP(uint16_t, status)
        LD2410_PROP(uint8_t, header)
        LD2410_PROP(uint8_t, maximum_distance_gate_n)
        LD2410_PROP(uint8_t, configure_maximum_moving_distance_gate)
        LD2410_PROP(uint8_t, configure_maximum_static_gate)
        LD2410_PROP(std::vector<uint8_t>, distance_gate_motion_sensitivity)
        LD2410_PROP(std::vector<uint8_t>, distance_gate_rest_sensitivity)
        LD2410_PROP(uint16_t, no_time_duration)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0161};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
            LD2410_READ_SHORT(header);
            LD2410_READ_SHORT(maximum_distance_gate_n);
            LD2410_READ_SHORT(configure_maximum_moving_distance_gate);
            LD2410_READ_SHORT(configure_maximum_static_gate);

            std::vector<uint8_t> b;
            b.resize(maximum_distance_gate_n()+1);
            for(size_t i = 0; i < b.size(); ++i) {
                b[i] = reader();
            }
            distance_gate_motion_sensitivity(b);

            for(size_t i = 0; i < b.size(); ++i) {
                b[i] = reader();
            }
            distance_gate_rest_sensitivity(b);
            LD2410_READ_SHORT(no_time_duration);
        }

        size_t size() const {
            size_t size_ = 0;
            size_ += sizeof(ReadParameterCommandAck::m_status);
            size_ += sizeof(ReadParameterCommandAck::m_header);
            size_ += sizeof(ReadParameterCommandAck::m_maximum_distance_gate_n);
            size_ += sizeof(ReadParameterCommandAck::m_configure_maximum_moving_distance_gate);
            size_ += sizeof(ReadParameterCommandAck::m_configure_maximum_static_gate);
            size_ += ReadParameterCommandAck::m_maximum_distance_gate_n;
            size_ += ReadParameterCommandAck::m_maximum_distance_gate_n;
            size_ += sizeof(ReadParameterCommandAck::m_no_time_duration);
            return size_;
        }
    };

    class ReadParameterCommand {
    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0061};
        using ack_t = ReadParameterCommandAck;

        void write(const writer_t &writer) const {

        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };

    class EnableEngineeringModeCommandAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0162};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(EnableEngineeringModeCommandAck::m_status);
            return size_;
        }
    };

    class EnableEngineeringModeCommand {
    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0062};
        using ack_t = EnableEngineeringModeCommandAck;

        void write(const writer_t &writer) const {

        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };

    class CloseEngineeringModeCommandAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0163};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(CloseEngineeringModeCommandAck::m_status);
            return size_;
        }
    };

    class CloseEngineeringModeCommand {
    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0063};
        using ack_t = CloseEngineeringModeCommandAck;

        void write(const writer_t &writer) const {

        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };

    class RangeSensitivityConfigurationCommandAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0164};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(RangeSensitivityConfigurationCommandAck::m_status);
            return size_;
        }
    };

    class RangeSensitivityConfigurationCommand {
        LD2410_PROP(uint16_t, distance_gate_word)
        LD2410_PROP(uint32_t, distance_gate_value)
        LD2410_PROP(uint16_t, motion_sensitivity_word)
        LD2410_PROP(uint32_t, motion_sensitivity_value)
        LD2410_PROP(uint16_t, static_sensitivity_word)
        LD2410_PROP(uint32_t, static_sensitivity_value)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x0064};
        using ack_t = RangeSensitivityConfigurationCommandAck;

        void write(const writer_t &writer) const {
            LD2410_WRITE_SHORT(distance_gate_word);
            LD2410_WRITE_SHORT(distance_gate_value);
            LD2410_WRITE_SHORT(motion_sensitivity_word);
            LD2410_WRITE_SHORT(motion_sensitivity_value);
            LD2410_WRITE_SHORT(static_sensitivity_word);
            LD2410_WRITE_SHORT(static_sensitivity_value);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(RangeSensitivityConfigurationCommand::m_distance_gate_word);
            size_ += sizeof(RangeSensitivityConfigurationCommand::m_distance_gate_value);
            size_ += sizeof(RangeSensitivityConfigurationCommand::m_motion_sensitivity_word);
            size_ += sizeof(RangeSensitivityConfigurationCommand::m_motion_sensitivity_value);
            size_ += sizeof(RangeSensitivityConfigurationCommand::m_static_sensitivity_word);
            size_ += sizeof(RangeSensitivityConfigurationCommand::m_static_sensitivity_value);
            return size_;
        }
    };

    class ReadFirmwareVersionCommandAck {
        LD2410_PROP(uint16_t, status)
        LD2410_PROP(uint16_t, firmware_type)
        LD2410_PROP(uint16_t, major_version_number)
        LD2410_PROP(uint32_t, minor_version_number)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x01a0};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
            LD2410_READ_SHORT(firmware_type);
            LD2410_READ_SHORT(major_version_number);
            LD2410_READ_SHORT(minor_version_number);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(ReadFirmwareVersionCommandAck::m_status);
            size_ += sizeof(ReadFirmwareVersionCommandAck::m_firmware_type);
            size_ += sizeof(ReadFirmwareVersionCommandAck::m_major_version_number);
            size_ += sizeof(ReadFirmwareVersionCommandAck::m_minor_version_number);
            return size_;
        }
    };

    class ReadFirmwareVersionCommand {

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x00a0};
        using ack_t = ReadFirmwareVersionCommandAck;

        void write(const writer_t &writer) const {
        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };

    class SetSerialPortBaudRateAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x01a1};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(SetSerialPortBaudRateAck::m_status);
            return size_;
        }
    };

    class SetSerialPortBaudRate {
        LD2410_PROP(uint16_t, baudRate_selection_index)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x00a1};
        using ack_t = SetSerialPortBaudRateAck;

        void write(const writer_t &writer) const {
            LD2410_WRITE_SHORT(baudRate_selection_index);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(SetSerialPortBaudRate::m_baudRate_selection_index);
            return size_;
        }
    };

    class FactoryResetAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x01a2};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(FactoryResetAck::m_status);
            return size_;
        }
    };

    class FactoryReset {

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x00a2};
        using ack_t = FactoryResetAck;

        void write(const writer_t &writer) const {
        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };

    class RestartModuleAck {
        LD2410_PROP(uint16_t, status)

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x01a3};

        void read(const ld2410::reader_t &reader) {
            LD2410_READ_SHORT(status);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(RestartModuleAck::m_status);
            return size_;
        }
    };

    class RestartModule {

    public:
        static inline constexpr to_bytes_union<uint32_t> definition_header{CommandHeader};
        static inline constexpr to_bytes_union<uint32_t> definition_mfr{CommandMFR};
        static inline constexpr to_bytes_union<uint16_t> definition_type{0x00a3};
        using ack_t = RestartModuleAck;

        void write(const writer_t &writer) const {
        }

        static const size_t size() {
            size_t size_ = 0;
            return size_;
        }
    };
}