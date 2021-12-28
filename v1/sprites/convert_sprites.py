#!/usr/bin/env python3
from PIL import Image
import glob

with open('converted_sprites.tsv', 'w') as o:
    for filepath in glob.iglob('*.png'):
        im = Image.open(filepath, 'r')
        width, height = im.size
        assert width == 8
        assert height == 8
        is_black_pixel = lambda pixel: int(pixel[3] == 255)
        bits_to_bytes = lambda bits: int("".join(map(str, bits)), 2)
        pixel_values = [is_black_pixel(p) for p in im.getdata()]
        byte_list = [bits_to_bytes(pixel_values[i:i+8]) for i in range(0, len(pixel_values), 8)]
        o.write(filepath + "\t" + ",".join([hex(b) for b in byte_list]) + "\n")
