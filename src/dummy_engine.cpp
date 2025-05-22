// dummy_engine.cpp
#include "dummy_engine.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <opencv2/opencv.hpp>

DummyEngine::DummyEngine() {}
DummyEngine::~DummyEngine() {}

void DummyEngine::configure(int nThreads) {
    omp_set_num_threads(nThreads);
}

std::vector<uint8_t> DummyEngine::process(const std::vector<uint8_t>& data, int width, int height) {
    // Input: flat grayscale image [size = width * height]
    cv::Mat src(height, width, CV_8UC1, const_cast<uint8_t*>(data.data()));

    // Prepare output image
    cv::Mat dst(height, width, CV_8UC1);

    // Tile splitting
    int x_split = width / 2;
    int y_split = height / 2;

    // Coordinates for 4 tiles: (x0,y0,x1,y1)
    std::vector<cv::Rect> tiles = {
        {0,           0,           x_split,       y_split},
        {x_split,     0,           width-x_split, y_split},
        {0,           y_split,     x_split,       height-y_split},
        {x_split,     y_split,     width-x_split, height-y_split}
    };

    // Store output tiles here
    std::vector<cv::Mat> results(4);

    std::vector<cv::Mat> results(4);
    tbb::parallel_for(
        tbb::blocked_range<int>(0, 4),
        [&](const tbb::blocked_range<int>& range) {
            for (int i = range.begin(); i != range.end(); ++i) {
                cv::Mat tile = src(tiles[i]).clone(); /
                // 1. Binarize (Otsu)
                cv::Mat bin;
                cv::threshold(tile, bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

                // 2. Morphology open
                cv::Mat morph;
                cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {3, 3});
                cv::morphologyEx(bin, morph, cv::MORPH_OPEN, kernel);

                results[i] = morph;
            }
        }
    );

    // Stitch result tiles into dst
    results[0].copyTo(dst(cv::Rect(0,         0,          tiles[0].width, tiles[0].height)));
    results[1].copyTo(dst(cv::Rect(x_split,   0,          tiles[1].width, tiles[1].height)));
    results[2].copyTo(dst(cv::Rect(0,         y_split,    tiles[2].width, tiles[2].height)));
    results[3].copyTo(dst(cv::Rect(x_split,   y_split,    tiles[3].width, tiles[3].height)));

    // Return as flat vector
    return std::vector<uint8_t>(dst.data, dst.data + dst.total());
}
