# WasmMOsDetector: Detecting Missed Optimizations in wasm-opt
This tool is currently under active development and refactoring.

## Code Structure
```txt
├── Contrast_Experiment_Test_Cases // C source code corresponding to detected issues, for contrast experiments with DITWO
├── Detected_Missed_Optimizations  // Detected missed optimizations
└── Tool                           // Tool
    ├── diff_testing_v1.py         // Differential testing script v1
    ├── instrumentator_v1.py       // Instrumentation script v1
    └── start_testing.py           // Start testing script
```
## Requirements

To run the differential testing framework, you need the following tools installed and accessible in your environment:

*   **Python 3.6+**
*   **[Csmith](https://github.com/csmith-project/csmith):** For generating random C programs.
*   **[Emscripten](https://emscripten.org/) (`emcc`):** For compiling C code to WebAssembly.
*   **[WABT](https://github.com/WebAssembly/wabt) (`wasm2wat`):** The WebAssembly Binary Toolkit, used for converting `.wasm` to `.wat`.
*   **[Binaryen](https://github.com/WebAssembly/binaryen) (`wasm-opt`):** The compiler infrastructure and optimizer for WebAssembly.

Ensuring these tools are in your system's `PATH` will make usage easier, though you can specify absolute paths via arguments.

## Usage

Use the start_testing.py script to start the automated testing workflow.

### Basic Command

Run the script with the required paths to `wasm-opt` and the Csmith include directory:

```bash
python3 start_testing.py \
    --wasm-opt /path/to/binaryen/bin/wasm-opt \
    --csmith-include /path/to/csmith/runtime/include/
```

### Full Options

You can customize the tool paths and the number of tests:

```bash
python3 start_testing.py \
    -n 100 \
    --csmith-include /usr/local/include/csmith \
    --wasm-opt /opt/binaryen/bin/wasm-opt \
    --csmith /usr/local/bin/csmith \
    --emcc /path/to/emsdk/upstream/emscripten/emcc \
    --wasm2wat /usr/bin/wasm2wat
```

### Arguments
*   `-n`, `--num_tests`: Number of test cases to generate and test (default: 1).
*   `--csmith-include`: **(Required)** Path to the directory containing Csmith headers (e.g., `csmith.h`).
*   `--wasm-opt`: **(Required)** Path to the `wasm-opt` executable.
*   `--csmith`: Path to the `csmith` executable (default: assumes `csmith` is in PATH).
*   `--emcc`: Path to the `emcc` executable (default: assumes `emcc` is in PATH).
*   `--wasm2wat`: Path to the `wasm2wat` executable (default: assumes `wasm2wat` is in PATH).
*   `--instrumentator_v1`: Path to the first instrumentation script (default: instrumentator_v1.py).