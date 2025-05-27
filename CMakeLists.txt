cmake_minimum_required(VERSION 3.13)
project(dummy_engine_wasm)

set(CMAKE_CXX_STANDARD 17)

set(TBB_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/tbb)
include_directories(${TBB_ROOT}/include)
set(TBB_LIB_DIR ${CMAKE_SOURCE_DIR}/thirdparty/tbb/build-emscripten/clang_20.0_cxx11_32_release)
# link_directories(${TBB_LIB_DIR})



set(CMAKE_EXECUTABLE_SUFFIX ".js")

include_directories(include)

add_executable(idengine_wasm
    src/dummy_engine.cpp
    src/bindings.cpp
    src/parallel_tools.cpp
)

# Pthread and Emscripten flags
# add_compile_options(-pthread)
# add_compile_options(-sSHARED_MEMORY=1)

# add_link_options(-pthread)
# add_link_options(-sPTHREAD_POOL_SIZE=4)
# add_link_options(-sSHARED_MEMORY=1)
# add_link_options(-sPTHREADS_DEBUG=1)
# add_link_options(-sEXIT_RUNTIME=0)
# add_link_options(-sEXPORTED_RUNTIME_METHODS=PThread)
# add_link_options(-sMODULARIZE=1)
# add_link_options(-sEXPORT_NAME=SmartIDEngine)
# add_link_options(-sENVIRONMENT=worker)
# add_link_options(-sALLOW_MEMORY_GROWTH=1)
# # add_link_options(-sFORCE_FILESYSTEM=1)
# add_link_options(-O3)
# add_link_options(-lembind)

target_link_libraries(idengine_wasm PRIVATE ${TBB_LIB_DIR}/libtbb.a embind)

target_compile_options(idengine_wasm PRIVATE -pthread -sSHARED_MEMORY=1)

set_target_properties(idengine_wasm PROPERTIES
    LINK_FLAGS "-sMODULARIZE -sEXPORT_NAME=SmartIDEngine -sENVIRONMENT=worker -sPTHREAD_POOL_SIZE=4 -pthread -lembind -sSHARED_MEMORY=1 -sPTHREADS_DEBUG=1 -sEXIT_RUNTIME=0 -sEXPORTED_RUNTIME_METHODS=PThread -sALLOW_MEMORY_GROWTH=1 -O3 -sENVIRONMENT=web,worker -sNO_FILESYSTEM -sNO_USE_SDL -sNO_USE_SDL_MIXER -sMINIMAL_RUNTIME_STREAMING_WASM_INSTANTIATION=1 -sDYNAMIC_EXECUTION=1 -sSTACK_SIZE=16mb -sINITIAL_MEMORY=196mb -sNO_ERROR_ON_UNDEFINED_SYMBOLS -sINCOMING_MODULE_JS_API=['locateFile','mainScriptUrlOrBlob','wasmMemory','buffer','instantiateWasm'] -sEXPORTED_FUNCTIONS=_malloc,_free,_start_tbb_session,_start_keepalive_mainloop,_stop_keepalive_mainloop"
)