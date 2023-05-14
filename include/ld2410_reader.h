#pragma once

#include <functional>

#include "ld2410_framework_switch.h"

namespace ld2410 {
    using reader_t = std::function<uint8_t()>;

    static inline uint16_t readUint16(const reader_t &r) {
        return r() | (r() << 8);
    }
}