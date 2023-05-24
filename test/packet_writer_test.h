#pragma once

#include <vector>
#include <tuple>

#include <gtest/gtest.h>
#include "ld2410_packet_writer.h"

#include <Arduino.h>

using namespace ld2410;

TEST(PacketWriterTest, Create) {
    PacketWriter w{};
    EXPECT_EQ(true, true);
}

class InMemoryWriterState {
public:
    std::vector<uint8_t> m_data{};
};

class InMemoryWriter {
public:
    std::shared_ptr<InMemoryWriterState> m_state;
    InMemoryWriter(): m_state(std::make_shared<InMemoryWriterState>()) {

    }

    void operator()(const uint8_t *data, size_t size) {
        for(size_t i = 0; i < size; i++) {
            m_state->m_data.push_back(data[i]);
        }
    }
};

inline void expect_same_vector(const std::vector<uint8_t> &expected, const std::vector<uint8_t> &actual) {
    EXPECT_EQ(expected.size(), actual.size());

    if (expected.size() == actual.size()) {
        for(size_t i = 0; i < actual.size(); i++) {
            EXPECT_EQ(actual[i], expected[i]);
        }
    }
}

struct WriterTestCase {
    PacketName name;
    std::vector<std::tuple<FieldName, uint32_t>> in_values;
    std::vector<uint8_t> expected;
};

inline void do_test_case(WriterTestCase test_case) {
    auto in_memory_writer = InMemoryWriter{};
    PacketWriter w{in_memory_writer};
    Packet packet = Packet(test_case.name);
    for(auto f: test_case.in_values) {
        packet.write(std::get<FieldName>(f), std::get<uint32_t>(f));
    }
    w.write(packet);

    expect_same_vector(test_case.expected, in_memory_writer.m_state->m_data);
}

TEST(PacketWriterTest, WriteEnableConfigurationCommand) {
    do_test_case({
        .name = PacketName::EnableConfigurationCommand,
        .in_values = {
            {FieldName::value, 1},
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, WriteEndConfigurationCommand) {
    do_test_case({
        .name = PacketName::EndConfigurationCommand,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xFE, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand) {
    do_test_case({
        .name = PacketName::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand,
        .in_values = {
            {FieldName::maximumMovingDistanceWord, 0},
            {FieldName::maximumMovingDistanceParameter, 8},
            {FieldName::maximumStaticDistanceDoorWord, 0x0001},
            {FieldName::maximumStaticDistanceDoorParameter, 8},
            {FieldName::noPersonDuration, 0x0002},
            {FieldName::sectionUnattendedDuration, 5},
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, ReadParameterCommand) {
    do_test_case({
        .name = PacketName::ReadParameterCommand,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x61, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, EnableEngineeringModeCommand) {
    do_test_case({
        .name = PacketName::EnableEngineeringModeCommand,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x62, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, CloseEngineeringModeCommand) {
    do_test_case({
        .name = PacketName::CloseEngineeringModeCommand,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x63, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}


TEST(PacketWriterTest, RangeSensitivityConfigurationCommand) {
    do_test_case({
        .name = PacketName::RangeSensitivityConfigurationCommand,
        .in_values = {
            {FieldName::distanceGateWord, 0},
            {FieldName::distanceGateValue, 0xffff},
            {FieldName::motionSensitivityWord, 1},
            {FieldName::motionSensitivityValue, 0x28},
            {FieldName::staticSensitivityWord, 2},
            {FieldName::staticSensitivityValue, 0x28},
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x64, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, ReadFirmwareVersionCommand) {
    do_test_case({
        .name = PacketName::ReadFirmwareVersionCommand,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA0, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, SetSerialPortBaudRate) {
    do_test_case({
        .name = PacketName::SetSerialPortBaudRate,
        .in_values = {
            {FieldName::baudRateSelectionIndex, 7},
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA1, 0x00, 0x07, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, FactoryReset) {
    do_test_case({
        .name = PacketName::FactoryReset,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA2, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, RestartModule) {
    do_test_case({
        .name = PacketName::RestartModule,
        .in_values = {
        },
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA3, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}