#pragma once

#include <functional>

#include <Arduino.h>

namespace ld2410 {
using writer_t = std::function<void(const std::vector<uint8_t> &data)>;

class StreamWriter {
  Stream *write_stream;
  

public:
    StreamWriter(Stream *write_stream): write_stream(write_stream) {

    }
    StreamWriter(): StreamWriter(&Serial) {

    }

    void operator()(const std::vector<uint8_t> &data) {
        if (write_stream == nullptr) return;

        auto data_ptr = data.data();
        auto data_size = data.size();

        while(true) {
            size_t written = write_stream->write(data.data(), data.size());
            if (written >= data_size) break;

            data_ptr += written;
            data_size -= written;
        };
    }
};
}