// dummy_engine.h
#pragma once
#include <vector>
#include <cstdint>

class DummyEngine {
public:
    DummyEngine();
    ~DummyEngine();

    void configure(int nThreads);

    // Accepts grayscale image (flat vector), width, height
    std::vector<uint8_t> process(const std::vector<uint8_t>& data, int width, int height);
};
