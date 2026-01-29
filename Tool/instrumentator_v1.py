import argparse
import sys
from collections import defaultdict


class Module:
    def __init__(self):
        self.sections = defaultdict(list)
        self.sections["function"] = {}

    def add_section(self, section_type, content):
        self.sections[section_type].append(content)

    def add_function(self, sig, body):
        self.sections["function"][sig] = body

    def dump(self):
        lines = ["(module"]
        section_order = [
            "type",
            "import",
            "function",
            "table",
            "memory",
            "global",
            "export",
            "start",
            "element",
            "elem",
            "data",
            "custom",
            "padding",
        ]
        for section in section_order:
            entries = self.sections.get(section, [])
            if section == "function":
                for sig, body in self.sections["function"].items():
                    lines.append(f"  {sig}")
                    for line in body:
                        lines.append(f"    {line}")
            else:
                for entry in entries:
                    for line in entry.split("\n"):
                        lines.append(f"  {line}")
        return "\n".join(lines)


def count_current_depth(line: str) -> int:
    open_count = 0
    close_count = 0
    in_string = False

    for char in line:
        if char == '"':
            in_string = not in_string  # Toggle string state
        elif not in_string:
            if char == "(":
                open_count += 1
            elif char == ")":
                close_count += 1
    return open_count - close_count


def parse_wat(text):
    module = Module()
    current_section = None
    current_content = []
    current_func_name = None
    func_body = []
    current_depth = 0
    section_map = {
        "type": "type",
        "import": "import",
        "func": "function",
        "table": "table",
        "memory": "memory",
        "global": "global",
        "export": "export",
        "start": "start",
        "element": "element",
        "elem": "elem",
        "data": "data",
        "custom": "custom",
    }

    for line in text.split("\n"):
        stripped = line.strip()

        if stripped.startswith("("):
            parts = stripped[1:].split(" ", 1)
            keyword = parts[0] if parts else ""
            if keyword in section_map:
                section = section_map[keyword]
                if section == "function":
                    current_func_name = stripped
                    func_body = []
                else:
                    current_section = section
                    current_content = []
                    current_depth = count_current_depth(
                        stripped
                    )  # stripped.count('(') - stripped.count(')')

        if current_func_name is not None:
            current_depth += count_current_depth(
                stripped
            )  # stripped.count('(') - stripped.count(')')
            if current_depth <= 0:
                if stripped != current_func_name:
                    func_body.append(stripped[:-1])
                    func_body.append(")")
                module.add_function(current_func_name, [ln for ln in func_body if ln])
                current_func_name = None
            elif stripped != current_func_name:
                func_body.append(stripped)

        elif current_section is not None:
            current_content.append(stripped)
            current_depth += count_current_depth(
                stripped
            )  # stripped.count('(') - stripped.count(')')
            if current_depth <= 0:
                content = "\n".join(current_content)
                module.sections[current_section].append(content)
                current_section = None
        elif stripped == ")":
            module.sections["padding"] = ")"

    return module


def instrument_module(module):
    instructions_point = {
        # Control Flow Instructions
        # "nop", "unreachable", "block", "loop", "if", "else", "br",
        # "br_if", "br_table", "return", "call", "call_indirect", "end",
        # "br_if"
        # "i32.const", "i64.const", "f32.const", "f64.const",
        # "local.get",
        # "local.set",
        "br_if"
        # "local.tee"
    }

    func_section = module.sections["function"]
    marker_count = 0
    new_body = []

    for func_name, func_body in func_section.items():  # Iterate with keys (names)
        new_body = []  # Create a *new* list for each function
        for instr in func_body:
            new_body.append(instr)

            # Check if the instruction is a control flow instruction
            instr_base = instr.split(" ")[0]

            if instr_base in instructions_point:
                # if instr_base != ')':
                new_body.append(f"call $marker_{marker_count}")
                marker_count += 1
        func_section[func_name] = new_body  # *Replace* the old body with the new one

    return marker_count


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Instrument a WAT file with markers.")
    parser.add_argument("input_file", help="Path to the input WAT file.")
    parser.add_argument(
        "-o", "--output_file", help="Path to the output WAT file.", default="output.wat"
    )  # default output file.
    args = parser.parse_args()

    try:
        with open(args.input_file, "r") as f:
            wat_text = f.read()

        module = parse_wat(wat_text)

        # Instrument marker invocations at each control flow point
        # Return the number of marker invocations added.
        marker_count = instrument_module(module)

        # Insert markers declarations
        for i in range(marker_count):
            module.sections["import"].append(
                f'(import "Marker" "marker_{i}" (func $marker_{i}))'
            )

        with open(args.output_file, "w") as outfile:
            outfile.write(module.dump())

    except FileNotFoundError:
        print(f"Error: Input file '{args.input_file}' not found.", file=sys.stderr)
        sys.exit(1)
    except ValueError as ve:
        print(f"Error: {ve}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred: {e}", file=sys.stderr)
        sys.exit(1)
