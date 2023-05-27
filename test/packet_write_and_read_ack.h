#pragma once

#include <vector>
#include <tuple>

#include <gtest/gtest.h>
#include "ld2410_packet_write_and_read_ack.h"

#include <Arduino.h>

using namespace ld2410;

class ZeroReader {
public:
    uint8_t operator()() {
        return 0;
    }
};

class NullWriter {
public:
    void operator()(const uint8_t *data, size_t size) {
        
    }
};

template <typename TCommand, typename TExpectedAck>
constexpr void check_ack_type() {
    static_assert(std::is_same<decltype(write_and_read_ack(NullWriter{}, ZeroReader{}, TCommand{})), std::optional<TExpectedAck>>::value, "wrong response type");
}

TEST(PacketWriterAndReadTest, CompilerCheck) {
    EnableConfigurationCommand p;
    p.value(1);
    
    check_ack_type<EnableConfigurationCommand, EnableConfigurationCommandAck>();
    check_ack_type<RestartModule, RestartModuleAck>();
    check_ack_type<FactoryReset, FactoryResetAck>();
    check_ack_type<SetSerialPortBaudRate, SetSerialPortBaudRateAck>();
    check_ack_type<ReadFirmwareVersionCommand, ReadFirmwareVersionCommandAck>();
    check_ack_type<RangeSensitivityConfigurationCommand, RangeSensitivityConfigurationCommandAck>();
    check_ack_type<CloseEngineeringModeCommand, CloseEngineeringModeCommandAck>();
    check_ack_type<EnableEngineeringModeCommand, EnableEngineeringModeCommandAck>();
    check_ack_type<ReadParameterCommand, ReadParameterCommandAck>();
    check_ack_type<MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand, MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck>();
    check_ack_type<EndConfigurationCommand, EndConfigurationCommandAck>();
    check_ack_type<EnableConfigurationCommand, EnableConfigurationCommandAck>();
}

TEST(PacketWriterAndReadTest, EnableConfigurationCommand) {
    EnableConfigurationCommand p;
    p.value(1);
    InMemoryReader r{{0xFD, 0xFC, 0xFB, 0xFA, 0x08, 0x00, 0xFF, 0x01, 0x01, 0x00, 0x01, 0x00, 0x40, 0x00, 0x04, 0x03, 0x02, 0x01}};
    NullWriter w{};

    auto resp = write_and_read_ack(w, r, p, 5000);
    EXPECT_EQ(true, resp.has_value());
    if (!resp.has_value()) return;

    EXPECT_EQ(1, resp->status());
}

TEST(PacketWriterAndReadTest, TimedOutEnableConfigurationCommand) {
    EnableConfigurationCommand p;
    p.value(1);
    ZeroReader r{};
    NullWriter w{};

    auto begin = millis();
    auto resp = write_and_read_ack(w, r, p, 500);
    EXPECT_EQ(false, resp.has_value());

    EXPECT_GE(500, millis() - begin);
    
}
