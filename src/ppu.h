#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "SDL.h"

typedef struct {
    SDL_Renderer* renderer;
    uint32_t palette[256];
    int scroll_x;
    int scroll_y;
} PPU;

void ppu_init(PPU* ppu,uint8_t* mem,SDL_Renderer* renderer);
void ppu_render_frame(PPU* ppu);
void ppu_set_scroll(PPU* ppu,int x,int y);
#endif
