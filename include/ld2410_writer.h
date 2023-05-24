#pragma once

#include <functional>

#include "ld2410_framework_switch.h"

namespace ld2410 {
    using writer_t = std::function<void(const uint8_t *data, size_t size)>;

    template <typename T>
    static inline void write_to_writer(writer_t &w, const T &val) {
        union
        {
            T u;
            uint8_t u8[sizeof(T)];
        } dest;

        dest.u = val;

        w(&dest.u8[0], sizeof(T));
    }

    template <typename T>
    static inline void write_to_writer_swap_endian(writer_t &w, const T &val) {
        union
        {
            T u;
            uint8_t u8[sizeof(T)];
        } dest;

        dest.u = val;

        for(size_t i = 0; i < sizeof(T)/2; i++) {
            auto tmp = dest.u8[i];
            dest.u8[i] = dest.u8[sizeof(T)-i-1];
            dest.u8[sizeof(T)-i-1] = tmp;
        }

        w(&dest.u8[0], sizeof(T));
    }
}