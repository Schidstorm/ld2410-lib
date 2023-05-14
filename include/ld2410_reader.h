#pragma once

#include <functional>

#include <Arduino.h>

namespace ld2410 {

using reader_t = std::function<uint8_t()>;

static inline uint16_t readUint16(reader_t &r) {
    return r() | (r() << 8);
}

template<std::size_t buffer_size = 32>
class StreamReaderState {
public:
    std::array<uint8_t, buffer_size> data;
    size_t restSize;
    size_t index;

    StreamReaderState(): restSize(0) {

    }
};

template<std::size_t buffer_size = 64>
class StreamReader {
  std::shared_ptr<StreamReaderState<buffer_size>> state;
  Stream *read_stream;
  

public:
    StreamReader(Stream *read_stream): state(std::shared_ptr<StreamReaderState<buffer_size>>(new StreamReaderState<buffer_size>())), read_stream(read_stream) {

    }
    StreamReader(): StreamReader(&Serial) {

    }

    uint8_t operator()() {
        if (read_stream == nullptr) return 0;

        if (state->restSize == 0) {
            auto red = read_stream->readBytes(state->data.data(), state->data.size());
            state->restSize = red;
            if (state->restSize == 0) return 0;
            state->index = 0;
        }

        --state->restSize;
        return state->data[state->index++];
    }
};

}