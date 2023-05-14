#pragma once

#include <functional>

#include "ld2410_arduino.h"

namespace ld2410 {
    using writer_t = std::function<void(const std::vector<uint8_t> &data)>;
}