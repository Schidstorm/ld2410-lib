#pragma once

#include <functional>

#include "ld2410_arduino.h"

namespace ld2410 {

using reader_t = std::function<uint8_t()>;

static inline uint16_t readUint16(reader_t &r) {
    return r() | (r() << 8);
}
}