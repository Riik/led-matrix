#!/usr/bin/python3
# font obtained from https://opengameart.org/content/dos-8x8-font
spacing = 1
grid_width = 16
grid_height = 16
import numpy as np

from PIL import Image
filepath = "font.png"
im = Image.open(filepath, 'r')
im_width, im_height = im.size
assert im_width == grid_width*8 + (grid_width+1)
assert im_height == grid_height*8 + (grid_height+1)
is_white_pixel = lambda pixel: int(pixel == (255, 255, 255, 255))
pixel_list = [is_white_pixel(p) for p in im.getdata()]
pixel_grid = np.array(pixel_list).reshape(im_width, im_height)
bits_to_byte = lambda bits: int("".join(map(str, bits)), 2)

output = []
for row in range(grid_height):
    for col in range(grid_width):
        x_start = 1 + row + row*8
        y_start = 1 + col + col*8
        char_data = pixel_grid[x_start:x_start+8, y_start:y_start+8]
        # np.roll is to compensate for wrong row order on pcb layout
        hex_string = ",".join([hex(bits_to_byte(np.roll(byte, 1))) for byte in char_data.T])
        output.append(hex_string)
with open("font_converted.tsv", 'w') as o, open("c_array.txt", 'w') as ot:
    for idx, hex_string in enumerate(output):
        o.write("\t".join([str(idx), chr(idx), hex_string]) + "\n")
    ot.write("{\n")
    for idx, hex_string in enumerate(output):
            # ot.write("\t{" + hex_string + "},\n")
            ot.write("\t" + hex_string + ",\n")
    ot.write("};")
print(f"Wrote {len(output)} characters of 8 bytes each")
