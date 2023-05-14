#pragma once

#include <memory>

#include "ld2410_packet_definition.h"
#include "ld2410_packet_identifier.h"
#include "ld2410_writer.h"
#include "ld2410_reader.h"
#include "ld2410_packet.h"
#include "ld2410_packet_reader.h"

namespace ld2410 {

const unsigned long default_timeout_ms = 4000;

class PacketWriter {
private:
    writer_t m_writer;

    const uint16_t packet_header_size = 4;
    const uint16_t packet_datasize_size = 2;
    const uint16_t packet_data_type_size = 2;

public:
    explicit PacketWriter(decltype(m_writer) writer): m_writer(writer) {

    }

#ifdef I_LD2410_ARDUINO
    PacketWriter(): PacketWriter(StreamWriter()) {
        
    }
#endif

    std::optional<Packet> write_and_wait_response(const Packet &packet, PacketReader &reader, unsigned long timeout_ms = default_timeout_ms) {
        if (!write(packet)) {
            return std::nullopt;
        }

        auto fail_time = millis() + timeout_ms;

        while(true) {
            if (millis() >= fail_time) return std::nullopt;

            auto ackpacket = reader.read();
            if (!ackpacket.has_value()) continue;

            auto expected_name = packetNameAck(ackpacket->packet_name());
            if (ackpacket->name()  == expected_name) {
                return ackpacket;
            }
        }

        return std::nullopt;
    }

    bool write(const Packet &packet) {
        auto &data_buffer = packet.data_buffer();
        uint16_t packet_type = packet.type();
        std::vector<uint8_t> write_buffer;
        write_buffer.resize(packet_header_size + packet_datasize_size + packet_data_type_size);

        write_to_buffer(write_buffer, 0, swap_endian(packet.header()));
        write_to_buffer(write_buffer, 4, ((uint16_t)(packet_data_type_size + data_buffer.size())));
        write_to_buffer(write_buffer, 6, (uint8_t)(packet_type&255));
        write_to_buffer(write_buffer, 7, (uint8_t)(packet_type >> 8));
        m_writer(write_buffer);
        m_writer(data_buffer);
        write_buffer.resize(packet_header_size);
        write_to_buffer(write_buffer, 0, swap_endian(packet.mfr()));
        m_writer(write_buffer);

        return true;
    }

private:
    template <typename T>
    void write_to_buffer(std::vector<uint8_t> &buffer, const size_t buffer_offset, const T val) const {
        write_to_buffer(buffer, buffer_offset, (uint8_t*)((void*)&val), sizeof(T));
    }

    static void write_to_buffer(std::vector<uint8_t> &buffer, const size_t buffer_offset, const uint8_t* val, const size_t size) {
        for(size_t i = 0; i < size; i++) {
            buffer[buffer_offset + i] = val[i];
        }
    }


    template <typename T>
    static T swap_endian(T u)
    {
        union
        {
            T u;
            unsigned char u8[sizeof(T)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(T); k++)
            dest.u8[k] = source.u8[sizeof(T) - k - 1];

        return dest.u;
    }
};

}