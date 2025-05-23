// dummy_engine.cpp
#include "dummy_engine.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include "image_proc.h"
#include <iostream>

DummyEngine::DummyEngine() {}
DummyEngine::~DummyEngine() {}

void DummyEngine::configure(int nThreads) {
    std::cout << "Configure" << std::endl;
}

ProcessResult DummyEngine::process(const std::vector<uint8_t>& data, int width, int height, int channels, int targetWidth) {
    // 1. Grayscale conversion
    int size = width * height;
    std::vector<uint8_t> gray(size);
    to_grayscale(data.data(), gray.data(), width, height, channels);

    // 2. Rescale
    
    int new_w = targetWidth;
    int new_h = static_cast<int>(height * (new_w / static_cast<float>(width)));
    std::vector<uint8_t> gray_scaled;


    if (new_w > width) {
        new_w = width;
        new_h = height;
        gray_scaled.resize(width * height);
        gray_scaled = std::move(gray);
    } else {
        gray_scaled.resize(new_w * new_h);
        rescale(gray.data(), gray_scaled.data(), width, height, new_w, new_h);
    }


    // 3. Split into 4 tiles
    int x_split = new_w / 2;
    int y_split = new_h / 2;
    std::vector<std::tuple<int, int, int, int>> tiles = {
        {0, 0, x_split, y_split},
        {x_split, 0, new_w - x_split, y_split},
        {0, y_split, x_split, new_h - y_split},
        {x_split, y_split, new_w - x_split, new_h - y_split}
    };

    std::vector<std::vector<uint8_t>> results(4);
    tbb::parallel_for(0, 4, [&](int i){
        auto [x0, y0, w, h] = tiles[i];
        std::vector<uint8_t> tile(w * h);

        // Copy tile data from scaled image
        for (int y = 0; y < h; ++y)
            std::copy_n(gray_scaled.data() + (y0 + y) * new_w + x0, w, tile.data() + y * w);

        std::vector<uint8_t> bin(w * h), morph(w * h);
        threshold(tile.data(), bin.data(), w, h, 200);
        morph_open(bin.data(), morph.data(), w, h);

        results[i] = std::move(morph);
    });

    // 4. Stitch processed tiles back together
    std::vector<uint8_t> dst(new_w * new_h, 0);
    for (int i = 0; i < 4; ++i) {
        auto [x0, y0, w, h] = tiles[i];
        for (int y = 0; y < h; ++y)
            std::copy_n(results[i].data() + y * w, w, dst.data() + (y0 + y) * new_w + x0);
    }

    // 5. Optionally: debug print some output
    for (int i = 0; i < 5; ++i)
        std::cout << "Pixels " << static_cast<int>(dst[i]) << std::endl;

    // 6. Return the scaled and processed image (client code needs to expect new_w x new_h image)
    // return dst;

    ProcessResult result;
    result.image = std::move(dst);
    result.width = new_w;
    result.height = new_h;
    return result;
}