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


## Analyze Historical MO Issues

To conduct this analysis, we systematically examined all 1,644 issues from the wasm-opt GitHub repository (November 2015 – May 2026). Because our study specifically targets missed optimizations (MOs) rather than functional correctness, we filtered the historical issues through a rigorous two-stage process: 1. From 1,644 total issues, we identified 210 as broadly optimization-related (mentioning optimization passes, missed optimizations, miscompilations, or suboptimal code generation). 2. From these 210, we excluded correctness bugs (miscompilations), crashes/assertion failures, feature requests (asking for new capability), unconfirmed reports, and general discussions/questions.

Below are summaries of how these 36 historical MOs align with our categories:

MOs matched:

|Class| Matching MOs | Fits|
|------|--------------|-----|
|Class 1 Overly Conservative Peephole Optimization|5.6% (2/36)|#6930, #6685|
|Class II Overly Conservative Block Merging|2.78% (1/36)|#84|
|Class IV Insufficient Branch Elimination|5.6% (2/36)|#4648, #4148|
|Class V Others (rule coverage, pass scheduling, feature maturity) |27.78% (10/36)|#3077, #5745, #5076, #4948, #2951, #2417, #2144, #1172, #62, #17|

MOs not matched:
|Class| Matching MOs | Fits|
|------|--------------|-----|
|About Wasm feature (GC, Asyncify, Multivalue, Type)|33.33% (12/36)|#8016, #8641, #8002, #4681, #5755, #8618, #8009, #7756, #5933, #5923, #5720, #5932|
|Inlining heuristic|5.6% (2/36)|#1364, #8619|
|Beyond wasm-opt ability && Not Necessary|19.44% (7/36)|#4947, #4274, #8312, #5003, #2724, #2133, #4950|

In total, about 15 of 36 fit our five classes. The remaining ~21 reveal two patterns our category does not fully capture: (1) GC type-system conservatism (TypeSSA not handling final types, rec group collisions, AbstractTypeRefining gaps), and (2) inlining heuristics and pass interactions (inlining decisions, inlining vs. asyncify/tail-call). These gaps are consistent with our acknowledged scope limitation: our Csmith-based pipeline tests MVP Wasm only and cannot reach GC or advanced feature interactions.

On the one hand, the analysis (15 of 36 matches) confirms that our findings are representative of real-world Wasm issues. On the other hand, our findings also reveals several MOs categories that have not been fully explored in prior research/issues, such as Class 2 and 3,

### Appendix: Issue Crawling

We used the GitHub official issue crawling method `gh` with the following options.

```bash
gh issue list -R WebAssembly/binaryen --state all --limit 10000 \
--json number,title,state,createdAt,url,body,author > issues.json
```

### Appendix: Complete List of 36 Historical MOs with Class Labels and Concise Development Context

- Class 1 Overly Conservative Peephole Optimization: (2)
	- #6930: "the ordering issue when the first arm has effects"
	- #6685: "in general we know we are missing a great deal of such patterns, ..., the specific pattern of `x >= 0` (unsigned)"
- Class II — Overly Conservative Block Merging (1)
	- #84: Lack of "Remove `br` at a block tail.", "RemoveUnusedBrs was not handling values. But it definitely can"
- Class IV Insufficient Branch Elimination (2)
	- #4648: "A null target is not a valid name so nothing can branch to there."
	- #4168: "Could we eliminate redundant (double) branch checks for same predicate"
- Class V Others (rule coverage, pass scheduling, feature maturity) (10)
	- #3077: "Shrinking values for small constant integers during store operation"
	- #5745: "exporting an empty function adds an unnecessary `nop` instruction."
	- #5076: "const->add->load/store pattern not optimized"
	- #4948: "empty function is not inlined/removed"
	- #2951: "redundant second cast to integer after already casted int value in asm.js"
	- #2417: "the `OptimizeInstructions` pass does not yet hand-optimize i64 instructions as much as it optimizes i32 instructions."
	- #2144: "miss things like an isnan function (which does `x != x` on the input) being worth inlining if all the callsites have a constant value sent."
	- #1172: "We should add one. The closest thing we have is Precompute, which should probably also do constant propagation"
	- #62: "Take advantage of the wider range of expressions in wasm"
	- #17: "not folding away some redundant type coercions"
- About Wasm feature (GC, Asyncify, Multivalue, Type) : #8016, #8641, #8002, #4681, #5755, #8618, #8009, #7756, #5933, #5923, #5720, #5932 (12)
- Inlining heuristic: (2)
	- #1364: The current inlining heuristics are pretty conservative.
	- #8619: "Inlining of large functions with only one caller"
- Beyond $\text{wasm-opt}$ ability && Not Necessary: (7)
	- #4947: "global variable is not optimized"
	- #4274: "So far it has been low priority due to little information on how beneficial it would be, and it is quite complex."
	- #8312: "it ended up too complex."
	- #5003: "We could in theory do it there, but we don't"
	- #2724: "Closing because the requested feature is no longer necessary."
	- #2133: "Closing because it looks like it's not actionable."
	- #4950: "Binaryen doesn't perform optimizations like that atm."

## Performance Impact Experience

The Setup part describes the experimental design on 21 benchmarks from the Emscripten benchmark suite, which is used by $\text{wasm-opt}$ for official benchmarking and includes real-world applications such as physics engines, compression libraries, and virtual machines. To isolate the effect of the $\text{wasm-opt}$ backend, we compiled each benchmark with $\text{emcc} \text{-O0}$ and then optimized the generated Wasm binaries with $\text{wasm-opt} \text{-O3}$. We compared $\text{wasm-opt} (\text{v127})$ against a baseline version in which the 17 explicit fix commits corresponding to our reported missed optimizations were reverted. The three silently fixed cases were excluded because their fixes could not be isolated as separate commits. For execution time, we ran each benchmark under Node.js five times and used the average.

| Benchmark | Mean Execution Time, Unoptimized (s) | Mean Execution Time, Optimized (s) | Speed Improvement (%) | Unoptimized Code Size (bytes) | Optimized Code Size (bytes) | Code Size Reduction (%) |
| :--------- | :------------------------------------ | :---------------------------------- | --------------------- | ---------------------------- | --------------------------- | ----------------------- |
| test_base64 | 1.336 | 1.342 | -0.45 | 51301 | 49365 | +3.77 |
| test_conditionals | 0.945 | 0.987 | -4.44 | 30188 | 30188 | 0.00 |
| test_copy | 16.201 | 16.088 | +0.70 | 31258 | 31258 | 0.00 |
| test_corrections | 3.318 | 3.604 | -8.62 | 30322 | 30322 | 0.00 |
| test_corrections64 | 1.326 | 1.423 | -7.32 | 30327 | 30327 | 0.00 |
| test_fannkuch | 3.830 | 3.790 | +1.04 | 49689 | 47753 | +3.90 |
| test_fasta_float | 4.440 | 4.391 | +1.10 | 52338 | 50406 | +3.69 |
| test_havlak | 8.166 | 8.077 | +1.09 | 91211 | 89272 | +2.13 |
| test_ifs | 2.963 | 2.945 | +0.61 | 30274 | 30274 | 0.00 |
| test_matrix_multiply | 3.730 | 3.729 | +0.03 | 46784 | 46309 | +1.02 |
| test_memops | 18.172 | 17.634 | +2.96 | 43105 | 42630 | +1.10 |
| test_primes_nocheck | 1.495 | 1.539 | -2.94 | 30175 | 30175 | 0.00 |
| test_skinning | 8.228 | 8.235 | -0.09 | 46583 | 46108 | +1.02 |
| test_zzz_box2d | 14.409 | 14.315 | +0.65 | 148243 | 146303 | +1.31 |
| test_zzz_bullet | 14.461 | 14.242 | +1.51 | 877123 | 875165 | +0.22 |
| test_zzz_coremark | 9.000 | 8.926 | +0.82 | 61811 | 59880 | +3.12 |
| test_zzz_linpack | 3.438 | 3.424 | +0.41 | 66177 | 64241 | +2.93 |
| test_zzz_lua_binarytrees | 4.252 | 4.249 | +0.07 | 376776 | 374839 | +0.51 |
| test_zzz_lua_scimark | 4.767 | 4.753 | +0.29 | 387127 | 385190 | +0.50 |
| test_zzz_lzma | 5.013 | 4.956 | +1.14 | 103730 | 101794 | +1.87 |
| test_zzz_sqlite | 3.116 | 3.100 | +0.51 | 964153 | 962202 | +0.20 |
| **Average** | - | - | **-0.52** | - | - | **+1.30** |