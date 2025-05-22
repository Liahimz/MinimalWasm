cmake_minimum_required(VERSION 3.13)
project(dummy_engine_wasm)

set(CMAKE_CXX_STANDARD 17)

set(TBB_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/tbb)
include_directories(${TBB_ROOT}/include)
link_directories(${TBB_ROOT}/lib)

set(CMAKE_EXECUTABLE_SUFFIX ".js")

add_executable(idengine_wasm
    src/dummy_engine.cpp
    src/bindings.cpp
)

# Pthread and Emscripten flags
add_compile_options(-pthread)
add_compile_options(-sSHARED_MEMORY=1)

add_link_options(-pthread)
add_link_options(-sPTHREAD_POOL_SIZE=4)
add_link_options(-sSHARED_MEMORY=1)
add_link_options(-sPTHREADS_DEBUG=1)
add_link_options(-sEXIT_RUNTIME=0)
add_link_options(-sEXPORTED_RUNTIME_METHODS=PThread)
add_link_options(-sMODULARIZE=1)
add_link_options(-sEXPORT_NAME=SmartIDEngine)
add_link_options(-sENVIRONMENT=worker)
add_link_options(-sALLOW_MEMORY_GROWTH=1)
add_link_options(-sFORCE_FILESYSTEM=1)
add_link_options(-O3)

target_link_libraries(idengine_wasm PRIVATE tbb_static)

