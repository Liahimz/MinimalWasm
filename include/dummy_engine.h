// dummy_engine.h
#pragma once
#include <vector>
#include <cstdint>

class DummyEngine {
public:
    DummyEngine();                 // Constructor
    ~DummyEngine();                // Destructor

    void configure(int nThreads);  // Parallel setup

    int process(const std::vector<uint8_t>& data); // Process (dummy: sum)
};
