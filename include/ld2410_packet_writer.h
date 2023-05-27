#pragma once


#include "ld2410_writer.h"

namespace ld2410 {
    template<typename T>
    void write_to_writer(const writer_t &writer, const T &packet) {
        uint32_t header = T::header.val.val;
        uint16_t type = T::type.val.val;
        uint32_t mfr = T::mfr.val.val;
        size_t size = packet.size();
        uint16_t data_size = sizeof(type) + size;

        write_any(writer, header);
        write_any(writer, data_size);
        write_any(writer, type);
        packet.write(writer);
        write_any(writer, mfr);
    }
}