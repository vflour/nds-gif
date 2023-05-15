#include <nds.h>
#include <gif.h>

#define FPI 6

void allocate_gif(gif_sprite *sprite) {
    for (int i = 0; i < SPRITE_SLOTS; i++) {
        sprite->sprite_gfx_mem_slots[i] = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    }
}

void init_gif(gif_sprite *sprite) {
    allocate_gif(sprite);
    FRAME_SET_SYMBOLS
}

void animate_gif(gif_sprite *sprite) {
    int frame = sprite->frame;
    u8* gfx = (u8*)sprite->frame_gfx_set[frame];

    for (int i = 0; i < SPRITE_SLOTS; i++) {
        u8* offset = gfx + i * 32*32;
        dmaCopy(offset, sprite->sprite_gfx_mem_slots[i], 32*32);
    }
}

inline void draw_gif(gif_sprite *sprite) {
    int x,y;
    for (int i = 0; i < SPRITE_SLOTS; i++) {
        x = (i % 8) * 32;
        y = (i / 8) * 32;
        oamSet(&oamMain, i, x, y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			sprite->sprite_gfx_mem_slots[i], -1, false, false, false, false, false);
    }
}

// main function
// initializes + draws the gif in an infinite loop

int main(void) {
    gif_sprite gif = {0};
    int frames = 0;

    // Init main screen to use the 2D engine
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

    // 
    init_gif(&gif);
    dmaCopy(GIF_FRAME_PALETTE, SPRITE_PALETTE, 512);

    while(1) {
		// scanKeys();
        animate_gif(&gif);
        frames++;
        if (frames%FPI == 0) {
            gif.frame++;
        }
        if (gif.frame >= MAX_FRAMES) {
            gif.frame = 0;
        }

        draw_gif(&gif);
            
        swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
    }
    
}