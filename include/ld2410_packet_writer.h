#pragma once


#include "ld2410_writer.h"

namespace ld2410 {
    template<typename T>
    void write_to_writer(const writer_t &writer, const T &packet) {
        uint32_t definition_header = T::definition_header.val.val;
        uint16_t definition_type = T::definition_type.val.val;
        uint32_t definition_mfr = T::definition_mfr.val.val;
        size_t size = packet.size();
        uint16_t data_size = sizeof(definition_type) + size;

        write_any(writer, definition_header);
        write_any(writer, data_size);
        write_any(writer, definition_type);
        packet.write(writer);
        write_any(writer, definition_mfr);
    }
}