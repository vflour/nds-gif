#! /usr/bin/python3
import sys
import glob

top_header = """
#pragma once
#define SPRITE_SLOTS 48
"""

bottom_header = """

typedef struct{
    int frame;

    u16* sprite_gfx_mem_slots[SPRITE_SLOTS];
	u8*  frame_gfx_set[MAX_FRAMES];

}gif_sprite;

void init_gif_gfx(int i, u8 *gfx, gif_sprite *sprite) {
	sprite->frame_gfx_set[i] = (u8*)gfx;
}

"""

def write_header_file(files, symbol):
    with open("gif.h", "wt") as gif_header:
        gif_header.write(top_header)
        gif_header.write(f"#include <{symbol}.h>\n")
        gif_header.write(f"#define GIF_FRAME_PALETTE {symbol}SharedPal\n")
        gif_header.write(f"#define GIF_FRAME_TILES(N) {symbol}_##N##Tiles\n")
        gif_header.write(f"#define GIF_FRAME_SET(N) init_gif_gfx(N, (u8*)GIF_FRAME_TILES(N), sprite);\n")
        gif_header.write(f"#define MAX_FRAMES {len(files)}\n\n")
        gif_header.write(f"#define FRAME_SET_SYMBOLS \\\n")
        for i in range(len(files)):
            nl_append = "\\" if  i != len(files)-1 else ""
            gif_header.write(f"\tGIF_FRAME_SET({i}) {nl_append}\n")
        gif_header.write(bottom_header)
        
symbol = sys.argv[1]
blob = sys.argv[2]
files = glob.glob(blob)
write_header_file(files, symbol)
    