#pragma once

#include <gtest/gtest.h>
#include "ld2410_packet_writer.h"
#include "ld2410_packets.h"
#include "helpers.h"

#include <Arduino.h>

using namespace ld2410;



template<typename T>
struct WriterTestCase {
    T packet;
    std::vector<uint8_t> expected;
};

template<typename T>
inline void do_test_case(WriterTestCase<T> test_case) {
    InMemoryWriter w;
    write_to_writer<T>(w, test_case.packet);

    expect_same_vector(test_case.expected, w.m_state->m_data);
}

TEST(PacketWriterTest, WriteEnableConfigurationCommand) {
    EnableConfigurationCommand p;
    p.value(1);
    
    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, WriteEndConfigurationCommand) {
    EndConfigurationCommand p;
    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xFE, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand) {
    MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand p;
    p.maximum_moving_distance_word(0);
    p.maximum_moving_distance_parameter(8);
    p.maximum_static_distance_door_word(0x0001);
    p.maximum_static_distance_door_parameter(8);
    p.no_person_duration(0x0002);
    p.section_unattended_duration(5);

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, ReadParameterCommand) {
    ReadParameterCommand p;

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x61, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, EnableEngineeringModeCommand) {
    EnableEngineeringModeCommand p;

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x62, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, CloseEngineeringModeCommand) {
    CloseEngineeringModeCommand p;

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x63, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}


TEST(PacketWriterTest, RangeSensitivityConfigurationCommand) {
    RangeSensitivityConfigurationCommand p;
    p.distance_gate_word(0);
    p.distance_gate_value(0xffff);
    p.motion_sensitivity_word(1);
    p.motion_sensitivity_value(0x28);
    p.static_sensitivity_word(2);
    p.static_sensitivity_value(0x28);

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x64, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, ReadFirmwareVersionCommand) {
    ReadFirmwareVersionCommand p;

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA0, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, SetSerialPortBaudRate) {
    SetSerialPortBaudRate p;
    p.baudRate_selection_index(7);

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA1, 0x00, 0x07, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, FactoryReset) {
    FactoryReset p;

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA2, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}

TEST(PacketWriterTest, RestartModule) {
    RestartModule p;

    do_test_case<decltype(p)>({
        .packet = p,
        .expected = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA3, 0x00, 0x04, 0x03, 0x02, 0x01},
    });
}