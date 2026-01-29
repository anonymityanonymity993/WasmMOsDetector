import subprocess
import os
import argparse
import sys
import re
import itertools


def run_command(command, error_message, ignore_error=False):
    """Runs a shell command and handles errors."""
    try:
        process = subprocess.run(
            command, shell=True, check=True, capture_output=True, text=True
        )
        return process.stdout
    except subprocess.CalledProcessError as e:
        print(f"Error: {error_message}")
        print(f"Command: {command}")
        print(f"Return Code: {e.returncode}")
        print(f"Stdout: {e.stdout}")
        print(f"Stderr: {e.stderr}")
        if not ignore_error:
            sys.exit(1)
        else:
            return ""
    except FileNotFoundError as e:
        print(f"Error: {e.strerror} - '{e.filename}'")
        print(
            "Make sure the required tools (csmith, emcc, wasm2wat, wat2wasm, wasm-opt) are installed and in your PATH."
        )
        if not ignore_error:
            sys.exit(1)
        else:
            return ""


def parse_log_file(filepath):
    """Parses a log file and returns a set of markers."""
    markers = set()
    try:
        with open(filepath, "r") as f:
            for line in f:
                markers.add(int(line.strip()))
    except FileNotFoundError:
        print(f"Error: File not found: {filepath}")
        return set()  # Return an empty set if file not found
    except Exception as e:
        print(f"Error reading file {filepath}: {e}")
        return set()  # Return empty set on other errors
    return markers


def check_containment(log_files, output_log_file):
    """Checks the containment relationship between log files and writes inconsistencies."""
    inconsistencies_found = False
    with open(output_log_file, "w") as outfile:
        for i in range(len(log_files) - 1):
            file1 = log_files[i]
            file2 = log_files[i + 1]

            markers1 = parse_log_file(file1)
            markers2 = parse_log_file(file2)

            # print(f'markers1: {markers1}')
            # print(f'markers2: {markers2}')

            # assert len(markers1) > 0,  "The set 's' must not be empty."
            # assert len(markers2) > 0,  "The set 's' must not be empty."

            if not markers1.issuperset(markers2):
                missing_markers = (
                    markers2 - markers1
                )  # Find markers in file2 but not in file1
                outfile.write(f"Inconsistency: {file1} does not contain {file2}\n")
                outfile.write(f"  Missing markers in {file1}: {missing_markers}\n")
                inconsistencies_found = True
                print(f"Inconsistency: {file1} does not contain {file2}")
                print(f"  Missing markers in {file1}: {missing_markers}")
    if not inconsistencies_found:
        os.system(f"rm -f {output_log_file}")
        for file in log_files:
            os.system(f"rm -f {file}")
    return inconsistencies_found


def differential_testing(
    num_tests,
    csmith_path,
    emcc_path,
    wasm2wat_path,
    wat2wasm_path,
    wasm_opt_path,
    instrumentator_path,
    csmith_include_path,
    csmith_other_flags="",
):
    """Implements the differential testing workflow."""

    # Create directories
    for dir_name in ["c_pool", "wasm_pool", "wat_pool", "source_pool", "log_pool"]:
        os.makedirs(dir_name, exist_ok=True)

    # for i in range(1, num_tests + 1):
    for i in itertools.count(start=1):
        print(f"\n--- Processing test case {i} ---")

        c_file = f"./c_pool/random{i}.c"
        wasm_file = f"./wasm_pool/random{i}.wasm"
        wat_file = f"./wat_pool/random{i}.wat"
        instrumented_wat_file = f"./source_pool/random{i}.wat"
        instrumented_wasm_file = f"./source_pool/random{i}.wasm"

        # 1. Generate C code
        csmith_command = f"{csmith_path} {csmith_other_flags} > {c_file}"
        run_command(csmith_command, "Csmith failed to generate code.")

        # 2. Compile to WASM
        emcc_command = (
            f"{emcc_path} {c_file} -I{csmith_include_path} -g2 -O0 -o {wasm_file}"
        )
        run_command(emcc_command, "Emcc failed to compile.")

        # 3. Convert to WAT
        wasm2wat_command = f"{wasm2wat_path} {wasm_file} -o {wat_file}"
        run_command(wasm2wat_command, "Wasm2wat failed to convert.")

        # 4. Instrument the WAT file
        instrumentator_command = (
            f"python3 {instrumentator_path} {wat_file} -o {instrumented_wat_file}"
        )
        run_command(instrumentator_command, "Instrumentation failed.")

        # 5.  Convert instrumented WAT back to WASM
        # wat2wasm_command = f"{wat2wasm_path} {instrumented_wat_file} -o {instrumented_wasm_file} --debug-names"
        # run_command(wat2wasm_command, "Wat2wasm failed to convert instrumented WAT.")

        # 6. Optimize and collect marker calls
        optimization_levels = ["O2", "O3"]
        log_files = []
        for opt_level in optimization_levels:
            log_file = f"log_pool/random{i}_{opt_level}.log"
            log_files.append(log_file)
            wasm_opt_command = (
                f"{wasm_opt_path} {instrumented_wat_file} -{opt_level} -S -all -o - | "
                f"grep \"call \\$marker\" | cut -d_ -f2 | cut -d')' -f1 > {log_file}"
            )
            run_command(
                wasm_opt_command,
                f"Wasm-opt failed at optimization level {opt_level}.",
                ignore_error=True,
            )  # collect even on failure

        # 7. Check for inconsistencies in log files
        output_log_file = f"log_pool/check_random{i}.log"
        if check_containment(log_files, output_log_file):
            print("The diff testing detected the inconsistency")
        else:
            os.system(
                f"rm -rf {c_file} {wasm_file} {wat_file} {instrumented_wat_file} {instrumented_wasm_file}"
            )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Differential testing workflow for WebAssembly."
    )
    parser.add_argument(
        "-n",
        "--num_tests",
        type=int,
        default=1,
        help="Number of test cases to generate.",
    )
    parser.add_argument(
        "--csmith", type=str, default="csmith", help="Path to the csmith executable."
    )
    parser.add_argument(
        "--emcc", type=str, default="emcc", help="Path to the emcc executable."
    )
    parser.add_argument(
        "--wasm2wat",
        type=str,
        default="wasm2wat",
        help="Path to the wasm2wat executable.",
    )
    parser.add_argument(
        "--wat2wasm",
        type=str,
        default="wat2wasm",
        help="Path to the wat2wasm executable.",
    )
    parser.add_argument(
        "--wasm-opt",
        type=str,
        default="/home/xry/Workspace/WebAssembly/binaryen/bin/wasm-opt",
        help="Path to the wasm-opt executable.",
    )
    parser.add_argument(
        "--instrumentator",
        type=str,
        default="instrumentator_v1.py",
        help="Path to the instrumentator.py script.",
    )
    parser.add_argument(
        "--csmith-include",
        type=str,
        default="/home/xry/csmith/include/",
        help="Path to csmith include files.",
    )
    parser.add_argument(
        "--csmith-flags",
        type=str,
        default="",
        help="Other flags to csmith (e.g., '--no-volatiles --no-volatile-pointers').",
    )

    args = parser.parse_args()

    differential_testing(
        args.num_tests,
        args.csmith,
        args.emcc,
        args.wasm2wat,
        args.wat2wasm,
        args.wasm_opt,
        args.instrumentator,
        args.csmith_include,
        args.csmith_flags,
    )
