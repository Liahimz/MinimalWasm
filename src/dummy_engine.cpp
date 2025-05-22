// dummy_engine.cpp
#include "dummy_engine.h"
#include <omp.h>

DummyEngine::DummyEngine()  {}
DummyEngine::~DummyEngine() {}

void DummyEngine::configure(int nThreads) {
    omp_set_num_threads(nThreads);
}

int DummyEngine::process(const std::vector<uint8_t>& data) {
    long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return static_cast<int>(sum & 0x7FFFFFFF);
}
