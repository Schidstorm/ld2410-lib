#pragma once

#ifndef LD2410_MILLIS 
#define LD2410_MILLIS millis()
#endif

#include "ld2410_packet_writer.h"
#include "ld2410_packet_reader.h"

namespace ld2410 {
    
    template<typename T, typename TWriter>
    std::optional<typename T::ack_t> write_and_read_ack(TWriter &writer, const reader_t &reader, const T &packet, const decltype(LD2410_MILLIS) timeout = 5000) {
        write_to_writer(writer, packet);

        decltype(LD2410_MILLIS) timeout_on = LD2410_MILLIS + timeout;
        
        while(true) {
            if (LD2410_MILLIS >= timeout_on) return std::nullopt;

            std::optional<typename T::ack_t> result = read_from_reader<typename T::ack_t>(reader);
            if (result.has_value()) return result;
        }
        
    }
    
}