#pragma once

#include <Arduino.h>

namespace ld2410 {
class StreamWriter {
  Stream *write_stream;
  

public:
    explicit StreamWriter(Stream *write_stream): write_stream(write_stream) {

    }
    StreamWriter(): StreamWriter(&Serial) {

    }

    void operator()(const std::vector<uint8_t> &data) {
        if (write_stream == nullptr) return;

        auto data_size = data.size();

        while(true) {
            size_t written = write_stream->write(data.data(), data.size());
            if (written >= data_size) break;

            data_size -= written;
        };
    }
};

template<std::size_t buffer_size = 32>
class StreamReaderState {
public:
    std::array<uint8_t, buffer_size> data;
    size_t restSize;
    size_t index;

    StreamReaderState(): restSize(0), index(0) {

    }
};

template<std::size_t buffer_size = 64>
class StreamReader {
  std::shared_ptr<StreamReaderState<buffer_size>> state;
  Stream *read_stream;
  

public:
    explicit StreamReader(Stream *read_stream): state(std::shared_ptr<StreamReaderState<buffer_size>>(new StreamReaderState<buffer_size>())), read_stream(read_stream) {

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