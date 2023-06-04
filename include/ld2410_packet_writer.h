#pragma once


#include "ld2410_writer.h"

namespace ld2410 {
    template<typename T, typename TWriter>
    void write_to_writer(TWriter &writer, const T &packet) {
        const uint32_t definition_header = T::definition_header.val.val;
        const uint16_t definition_type = T::definition_type.val.val;
        const uint32_t definition_mfr = T::definition_mfr.val.val;
        const size_t size = packet.size();
        const uint16_t data_size = sizeof(definition_type) + size;

        write_any(writer, definition_header);
        write_any(writer, data_size);
        write_any(writer, definition_type);
        packet.write(writer);
        write_any(writer, definition_mfr);
    }
}