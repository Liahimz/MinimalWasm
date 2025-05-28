# MinimalWasmParallelImage

This is a minimal WebAssembly app that demonstrates parallel image processing in C++ using the [oneTBB](https://github.com/oneapi-src/oneTBB) library and Emscripten. The project is structured as a C++ library (no main), exposing a simple image processing function (morphological open) to JavaScript. All heavy computations are run in parallel inside a TBB task arena using `tbb::parallel_for`.

## Features

- Parallel image processing in C++ (morphological open)
- Uses TBB for parallelism (`tbb::parallel_for` inside `tbb::task_arena`)
- Designed for library-style use—no `main()`
- Minimal JS frontend; loads, processes, and displays the image in the browser
- Emscripten/WebAssembly backend

## Requirements

- **Emscripten** 3.1.74 or newer (must be activated: `source <emsdk>/emsdk_env.sh`)
- **Node.js** (for `npx serve`)
- **npx** (comes with Node.js)
- **Python 3** (for the build script)

## Quick Start

1. **Clone the repo and install prerequisites:**

   Make sure you have Emscripten 3.1.74+ set up and in your path.

2. **Build the project:**

   ```sh
   python3 build.py
   ```
   The script will:
    - Build C++ sources and WASM/JS via Emscripten
    - Build and link TBB for WASM if needed
    - Copy all needed JS/HTML files to `build/`
3. **Run**

  ```bash
    cd build
    npx serve
  ```

  Then open `sample.html` in your browser (for example, [http://localhost:3000/sample.html](http://localhost:3000/sample.html)).

## Project Structure

  .
  ├── build.py          # Build script
  ├── CMakeLists.txt
  ├── src/
  │   ├── dummy_engine.cpp
  │   ├── parallel_tools.cpp
  │   └── bindings.cpp
  ├── include/
  │   ├── dummy_engine.h
  │   ├── parallel_tools.h
  │   └── image_proc.h
  ├── samples/
  │   ├── serve.json
  │   ├── sample.html
  │   ├── app.js
  │   └── worker.js
  ├── thirdparty/
  │   └── tbb/          # TBB library (with WASM build)
  └── ...

## Notes

- The main C++ function is exposed via Embind and can be called from JavaScript.

- The build system auto-builds TBB for WebAssembly if not already present.

- If you get errors about missing files, double-check that you’ve run the build script and have the correct Emscripten environment active.