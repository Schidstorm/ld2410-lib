#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <gtest/gtest.h>
#include "ld2410_packet_reader.h"

#include <Arduino.h>

using namespace ld2410;

TEST(PacketReaderTest, Create) {
    PacketReader r{};
    EXPECT_EQ(true, true);
}

struct InMemoryReaderState {
    std::vector<uint8_t> m_data;
    uint16_t m_index;
};

class InMemoryReader {
private:
    std::shared_ptr<InMemoryReaderState> m_state;

public:
    InMemoryReader(std::vector<uint8_t> data): m_state(std::make_shared<InMemoryReaderState>(InMemoryReaderState{data, 0})) {

    }

    uint8_t operator()() {
        if (m_state->m_index >= m_state->m_data.size()) return 0;
        return m_state->m_data[m_state->m_index++];
    }
};

struct ReaderTestCase {
    std::vector<uint8_t> in_values;
    PacketName expected_name;
    std::vector<std::tuple<FieldName, uint32_t>> expected_fields;
};

inline void do_test_case(ReaderTestCase test_case) {
    PacketReader r{InMemoryReader{test_case.in_values}};
    auto packet = r.read();
    EXPECT_EQ(true, packet.has_value());
    EXPECT_EQ(test_case.expected_name, packet->name());
    std::map<FieldName, uint16_t> field_apprearances{};
    for(auto f: test_case.expected_fields) {
        auto v = packet->read_nth_field(std::get<FieldName>(f), field_apprearances[std::get<FieldName>(f)]++);
        EXPECT_TRUE(v.has_value());
        EXPECT_EQ(std::get<uint32_t>(f), *v);
    }
}

TEST(PacketReaderTest, ReadReportingDataFrame) {
    do_test_case({
        .in_values = {0xF4, 0xF3, 0xF2, 0xF1, 0x0D, 0x00, 0x02, 0xAA, 0x02, 0x51, 0x01, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x55, 0x00, 0xF8, 0xF7, 0xF6, 0xF5},
        .expected_name = PacketName::ReportingDataFrame,
        .expected_fields = {
            {FieldName::targetState, 0x02},
            {FieldName::movementTargetDistance, 0x0151},
            {FieldName::exerciseTargetEnergyValue, 0x00},
            {FieldName::stationaryTargetDistance, 0x0000},
            {FieldName::stationaryTargetEnergyValue, 0x3B},
            {FieldName::detectionDistance, 0x0000},
            {FieldName::tail, 0x55},
            {FieldName::check, 0x00},
        }
    });
}

TEST(PacketReaderTest, ReadReportingEngineeringDataFrame) {
    do_test_case({
        .in_values = {0xF4, 0xF3, 0xF2, 0xF1, 0x23, 0x00, 0x01, 0xAA, 0x03, 0x1E, 0x00, 0x3C, 0x00, 0x00, 0x39, 0x00, 0x00, 0x08, 0x08, 0x3C, 0x22, 0x05, 0x03, 0x03, 0x04, 0x03, 0x06, 0x05, 0x00, 0x00, 0x39, 0x10, 0x13, 0x06, 0x06, 0x08, 0x04, 0x03, 0x05, 0x55, 0x00, 0xF8, 0xF7, 0xF6, 0xF5},
        .expected_name = PacketName::EngineeringModeDataFrame,
        .expected_fields = {
            {FieldName::targetState, 3},
            {FieldName::movementTargetDistance, 0x1E},
            {FieldName::exerciseTargetEnergyValue, 0x3C},
            {FieldName::stationaryTargetDistance, 0},
            {FieldName::stationaryTargetEnergyValue, 0x39},
            {FieldName::detectionDistance, 0},
            {FieldName::maximumMovingDistanceGateN, 8},
            {FieldName::maximumStaticDistanceGateN, 8},
            {FieldName::movementDistanceGateEnergyValue, 0x3C},
            {FieldName::movementDistanceGateEnergyValue, 0x22},
            {FieldName::movementDistanceGateEnergyValue, 0x05},
            {FieldName::movementDistanceGateEnergyValue, 0x03},
            {FieldName::movementDistanceGateEnergyValue, 0x03},
            {FieldName::movementDistanceGateEnergyValue, 0x04},
            {FieldName::movementDistanceGateEnergyValue, 0x03},
            {FieldName::movementDistanceGateEnergyValue, 0x06},
            {FieldName::staticDistanceGateEnergyValue, 0x05},
            {FieldName::staticDistanceGateEnergyValue, 0x00},
            {FieldName::staticDistanceGateEnergyValue, 0x00},
            {FieldName::staticDistanceGateEnergyValue, 0x39},
            {FieldName::staticDistanceGateEnergyValue, 0x10},
            {FieldName::staticDistanceGateEnergyValue, 0x13},
            {FieldName::staticDistanceGateEnergyValue, 0x06},
            {FieldName::staticDistanceGateEnergyValue, 0x06},
        }
    });
}