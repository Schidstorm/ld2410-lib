#pragma once

#include <gtest/gtest.h>
#include "ld2410.h"
#include "helpers.h"

#include <Arduino.h>

using namespace ld2410;



TEST(PacketReaderTest, SingleCompileCheck) {
    InMemoryReader r{{}};
    ld2410::EngineeringModeDataFrame res1;
    ld2410::read_from_reader<ld2410::EngineeringModeDataFrame>(r);
}

TEST(PacketReaderTest, TwoCompileCheck) {
    InMemoryReader r{{}};
    ld2410::read_from_reader<ld2410::EngineeringModeDataFrame, ld2410::ReportingDataFrame>(r);
}

TEST(PacketReaderTest, ThreeCompileCheck) {
    InMemoryReader r{{}};
    ld2410::read_from_reader<ld2410::EngineeringModeDataFrame, ld2410::ReportingDataFrame, ReadParameterCommandAck>(r);
}

TEST(PacketReaderTest, ReadReportingDataFrame) {
    InMemoryReader r{{0xF4, 0xF3, 0xF2, 0xF1, 0x0D, 0x00, 0x02, 0xAA, 0x02, 0x51, 0x01, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x55, 0x00, 0xF8, 0xF7, 0xF6, 0xF5}};
    auto packet = ld2410::read_from_reader<ld2410::EngineeringModeDataFrame, ld2410::ReportingDataFrame>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(true, std::holds_alternative<ld2410::ReportingDataFrame>(*packet));
    auto t_packet = std::get<ld2410::ReportingDataFrame>(*packet);
    EXPECT_EQ(0x02, t_packet.target_state());
    EXPECT_EQ(0x0151, t_packet.movement_target_distance());
    EXPECT_EQ(0x00, t_packet.exercise_target_energy_value());
    EXPECT_EQ(0x0000, t_packet.stationary_target_distance());
    EXPECT_EQ(0x3B, t_packet.stationary_target_energy_value());
    EXPECT_EQ(0x0000, t_packet.detection_distance());
    EXPECT_EQ(0x55, t_packet.tail());
    EXPECT_EQ(0x00, t_packet.check());
}

TEST(PacketReaderTest, ReadReportingEngineeringDataFrame) {
    InMemoryReader r{{0xF4, 0xF3, 0xF2, 0xF1, 0x23, 0x00, 0x01, 0xAA, 0x03, 0x1E, 0x00, 0x3C, 0x00, 0x00, 0x39, 0x00, 0x00, 0x08, 0x08, 0x3C, 0x22, 0x05, 0x03, 0x03, 0x04, 0x03, 0x06, 0x05, 0x00, 0x00, 0x39, 0x10, 0x13, 0x06, 0x06, 0x08, 0x04, 0x03, 0x05, 0x55, 0x00, 0xF8, 0xF7, 0xF6, 0xF5}};
    auto packet = ld2410::read_from_reader<ld2410::EngineeringModeDataFrame, ld2410::ReportingDataFrame>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(true, std::holds_alternative<ld2410::EngineeringModeDataFrame>(*packet));
    auto t_packet = std::get<ld2410::EngineeringModeDataFrame>(*packet);
    EXPECT_EQ(3, t_packet.target_state());
    EXPECT_EQ(0x1E, t_packet.movement_target_distance());
    EXPECT_EQ(0x3C, t_packet.exercise_target_energy_value());
    EXPECT_EQ(0, t_packet.stationary_target_distance());
    EXPECT_EQ(0x39, t_packet.stationary_target_energy_value());
    EXPECT_EQ(0, t_packet.detection_distance());
    EXPECT_EQ(8, t_packet.movement_distance_gate_energy_value().size());
    EXPECT_EQ(0x3C, t_packet.movement_distance_gate_energy_value()[0]);
    EXPECT_EQ(0x22, t_packet.movement_distance_gate_energy_value()[1]);
    EXPECT_EQ(0x05, t_packet.movement_distance_gate_energy_value()[2]);
    EXPECT_EQ(0x03, t_packet.movement_distance_gate_energy_value()[3]);
    EXPECT_EQ(0x03, t_packet.movement_distance_gate_energy_value()[4]);
    EXPECT_EQ(0x04, t_packet.movement_distance_gate_energy_value()[5]);
    EXPECT_EQ(0x03, t_packet.movement_distance_gate_energy_value()[6]);
    EXPECT_EQ(0x06, t_packet.movement_distance_gate_energy_value()[7]);
    EXPECT_EQ(8, t_packet.static_distance_gate_energy_value().size());
    EXPECT_EQ(0x05, t_packet.static_distance_gate_energy_value()[0]);
    EXPECT_EQ(0x00, t_packet.static_distance_gate_energy_value()[1]);
    EXPECT_EQ(0x00, t_packet.static_distance_gate_energy_value()[2]);
    EXPECT_EQ(0x39, t_packet.static_distance_gate_energy_value()[3]);
    EXPECT_EQ(0x10, t_packet.static_distance_gate_energy_value()[4]);
    EXPECT_EQ(0x13, t_packet.static_distance_gate_energy_value()[5]);
    EXPECT_EQ(0x06, t_packet.static_distance_gate_energy_value()[6]);
    EXPECT_EQ(0x06, t_packet.static_distance_gate_energy_value()[7]);
}


TEST(PacketReaderTest, EnableConfigurationCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x08, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::EnableConfigurationCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
    EXPECT_EQ(1, packet->protocol_version());
}


TEST(PacketReaderTest, EndConfigurationCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::EndConfigurationCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, ReadParameterCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x18, 0x00, 0x61, 0x01, 0x00, 0x00, 0xaa, 0x08, 0x08, 0x08, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::ReadParameterCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
    EXPECT_EQ(0xaa, packet->header());
    EXPECT_EQ(8, packet->maximum_distance_gate_n());
    EXPECT_EQ(8, packet->configure_maximum_moving_distance_gate());
    EXPECT_EQ(8, packet->configure_maximum_static_gate());

    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[0]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[1]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[2]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[3]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[4]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[5]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[6]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[7]);
    EXPECT_EQ(0x14, packet->distance_gate_motion_sensitivity()[8]);
    
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[0]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[1]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[2]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[3]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[4]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[5]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[6]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[7]);
    EXPECT_EQ(0x19, packet->distance_gate_rest_sensitivity()[8]);

    EXPECT_EQ(1, packet->no_time_duration());
}

TEST(PacketReaderTest, EnableEngineeringModeCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x62, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::EnableEngineeringModeCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, CloseEngineeringModeCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x63, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::CloseEngineeringModeCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, RangeSensitivityConfigurationCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x64, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::RangeSensitivityConfigurationCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, ReadFirmwareVersionCommandAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x0B, 0x00, 0xA0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x16, 0x24, 0x06, 0x22, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::ReadFirmwareVersionCommandAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
    EXPECT_EQ(0, packet->firmware_type());
    EXPECT_EQ(0x0102, packet->major_version_number());
    EXPECT_EQ(0x22062416, packet->minor_version_number());
}

TEST(PacketReaderTest, SetSerialPortBaudRateAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA1, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::SetSerialPortBaudRateAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, FactoryResetAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA2, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::FactoryResetAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}

TEST(PacketReaderTest, RestartModuleAck) {
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA3, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01}};
    auto packet = ld2410::read_from_reader<ld2410::RestartModuleAck>(r);
    EXPECT_EQ(true, packet.has_value());
    if (!packet.has_value()) return;

    EXPECT_EQ(0, packet->status());
}
