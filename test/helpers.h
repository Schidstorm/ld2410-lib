#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include <gtest/gtest.h>

struct InMemoryReaderState {
    std::vector<uint8_t> m_data;
    uint16_t m_index;
};

class InMemoryReader {
private:
    std::shared_ptr<InMemoryReaderState> m_state;

public:
    InMemoryReader(std::vector<uint8_t> data): m_state(std::make_shared<InMemoryReaderState>(InMemoryReaderState{data, 0})) {

    }

    uint8_t operator()() {
        if (m_state->m_index >= m_state->m_data.size()) return 0;
        return m_state->m_data[m_state->m_index++];
    }
};



class InMemoryWriterState {
public:
    std::vector<uint8_t> m_data{};
};

class InMemoryWriter {
public:
    std::shared_ptr<InMemoryWriterState> m_state;
    InMemoryWriter(): m_state(std::make_shared<InMemoryWriterState>()) {

    }

    void operator()(const uint8_t *data, size_t size) {
        for(size_t i = 0; i < size; i++) {
            m_state->m_data.push_back(data[i]);
        }
    }
};

inline void expect_same_vector(const std::vector<uint8_t> &expected, const std::vector<uint8_t> &actual) {
    EXPECT_EQ(expected.size(), actual.size());

    if (expected.size() == actual.size()) {
        for(size_t i = 0; i < actual.size(); i++) {
            EXPECT_EQ(actual[i], expected[i]);
        }
    }
}