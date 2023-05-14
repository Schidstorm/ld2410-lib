#pragma once

#include <memory>

#include "ld2410_packet_definition.h"
#include "ld2410_packet_identifier.h"
#include "ld2410_packet.h"

namespace ld2410 {

class PacketReader {
private:
    std::unique_ptr<PacketIdentifier> m_packet_identifier;
    reader_t m_reader;

public:
    PacketReader(decltype(m_packet_identifier) &&packet_identifier, decltype(m_reader) reader): m_packet_identifier(std::move(packet_identifier)), m_reader(reader) {

    }

    PacketReader(decltype(m_reader) reader): m_packet_identifier(std::make_unique<PacketIdentifier>()), m_reader(reader) {

    }

    PacketReader(): PacketReader(StreamReader()) {
        
    }

    std::optional<Packet> read() {
        auto identified = m_packet_identifier->read(m_reader);
        if (!identified.has_value()) return std::nullopt;

        return Packet(identified->definition, read_bytes(identified->data_size - sizeof(uint16_t))); // subtract command word which gets already red by the identifer
    }

private:
    std::vector<uint8_t> read_bytes(uint16_t size) const {
        std::vector<uint8_t> result;
        result.reserve(size);
        for(uint16_t i = 0; i < size; i++) {
            result.push_back(m_reader());
        }
        return result;
    }
};

}