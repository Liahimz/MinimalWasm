cmake_minimum_required(VERSION 3.13)
project(dummy_engine_wasm)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)

set(CMAKE_EXECUTABLE_SUFFIX ".js")

add_executable(idengine_wasm
    dummy_engine.cpp
    bindings.cpp
)

target_link_options(idengine_wasm PRIVATE
    "SHELL:-sMODULARIZE=1"
    "SHELL:-sEXPORT_ES6=1"
    "SHELL:-sENVIRONMENT=worker"
    "SHELL:-sEXPORTED_RUNTIME_METHODS=ccall,cwrap"
    "SHELL:-sALLOW_MEMORY_GROWTH=1"
    "SHELL:-O3"
    "SHELL:-sFORCE_FILESYSTEM=1"
    "SHELL:-sUSE_PTHREADS=0"
)

target_link_libraries(idengine_wasm PRIVATE OpenMP::OpenMP_CXX)
