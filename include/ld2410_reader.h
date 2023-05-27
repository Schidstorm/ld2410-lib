#pragma once

#include <functional>

#include "ld2410_framework_switch.h"

namespace ld2410 {
    using reader_t = std::function<uint8_t()>;

    static inline uint16_t readUint16(const reader_t &r) {
        return r() | (r() << 8);
    }

    template <typename T>
    static inline T read_any_swapped(const reader_t &r) {
        union {
            T v;
            uint8_t u8[sizeof(T)];
        } res;

        for(size_t i = 0; i < sizeof(T); ++i) {
            res.u8[sizeof(T) - 1 - i] = r();
        }

        return res.v;
    }

    template <typename T>
    static inline T read_any(const reader_t &r) {
        union {
            T v;
            uint8_t u8[sizeof(T)];
        } res;

        for(size_t i = 0; i < sizeof(T); ++i) {
            res.u8[i] = r();
        }

        return res.v;
    }
}