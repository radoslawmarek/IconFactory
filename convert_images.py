#!/usr/bin/env python3
import os

def convert_png_to_cpp_array(png_file, var_name):
    with open(png_file, 'rb') as f:
        data = f.read()

    cpp_code = f"// Generated from {os.path.basename(png_file)}\n"
    cpp_code += f"static const unsigned char {var_name}_png[] = {{\n"

    for i in range(0, len(data), 16):
        chunk = data[i:i+16]
        cpp_code += "    "
        cpp_code += ", ".join(f"0x{b:02x}" for b in chunk)
        if i + 16 < len(data):
            cpp_code += ","
        cpp_code += "\n"

    cpp_code += "};\n"
    cpp_code += f"static const size_t {var_name}_png_size = {len(data)};\n\n"

    return cpp_code

# Convert images
header_content = """#pragma once

// Embedded image resources for IconFactory
// Auto-generated - do not edit manually

"""

header_content += convert_png_to_cpp_array("assets/fold-horizontal.png", "fold_horizontal")
header_content += convert_png_to_cpp_array("assets/fold-vertical.png", "fold_vertical")
header_content += convert_png_to_cpp_array("assets/minus.png", "minus_horizontal")
header_content += convert_png_to_cpp_array("assets/minus_rotated_90.png", "minus_vertical")
header_content += convert_png_to_cpp_array("assets/rectangle.png", "rectangle")
header_content += convert_png_to_cpp_array("assets/open.png", "open")
header_content += convert_png_to_cpp_array("assets/save.png", "save")
header_content += convert_png_to_cpp_array("assets/rotate-cw.png", "rotate_cw")
header_content += convert_png_to_cpp_array("assets/select_rectangle.png", "select_rectangle")
header_content += convert_png_to_cpp_array("assets/move.png", "move")
header_content += convert_png_to_cpp_array("assets/fill.png", "fill")

# Write to header file
with open("src/Resources.h", "w") as f:
    f.write(header_content)

print("Generated src/Resources.h")
print("  - fold_horizontal.png")
print("  - fold_vertical.png")
print("  - minus.png (horizontal line)")
print("  - minus_rotated_90.png (vertical line)")
print("  - rectangle.png")
print("  - open.png")
print("  - save.png")
print("  - rotate_cw.png")
print("  - select_rectangle.png")
print("  - move.png")
print("  - fill.png")
