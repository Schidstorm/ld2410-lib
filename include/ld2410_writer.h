#pragma once

#include <functional>

#include "ld2410_framework_switch.h"

namespace ld2410 {
    using writer_t = std::function<void(const std::vector<uint8_t> &data)>;
}