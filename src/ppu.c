#include "ppu.h"
#include <string.h>
#include "SDL.h"

static uint8_t* mem_ptr;

void ppu_init(PPU* ppu,uint8_t* mem,SDL_Renderer* renderer){
    ppu->renderer=renderer;
    mem_ptr=mem;
    ppu->scroll_x=0;
    ppu->scroll_y=0;
    for(int i=0;i<256;i++) ppu->palette[i]=0xFF000000|(i<<16)|(i<<8)|i;
}

void ppu_set_scroll(PPU* ppu,int x,int y){
    ppu->scroll_x=x;
    ppu->scroll_y=y;
}

static uint8_t get_4bpp_pixel(uint8_t* tiledata,int x,int y){
    uint8_t plane0=tiledata[y];
    uint8_t plane1=tiledata[y+8];
    uint8_t plane2=tiledata[y+16];
    uint8_t plane3=tiledata[y+24];
    return ((plane3>>(7-x))&1)<<3 | ((plane2>>(7-x))&1)<<2 | ((plane1>>(7-x))&1)<<1 | ((plane0>>(7-x))&1);
}

void ppu_render_frame(PPU* ppu){
    SDL_SetRenderDrawColor(ppu->renderer,0,0,0,255);
    SDL_RenderClear(ppu->renderer);

    int tiles_x=32;
    int tiles_y=32;
    int tile_size=8;

    for(int ty=0;ty<tiles_y;ty++){
        for(int tx=0;tx<tiles_x;tx++){
            uint8_t tile_index=mem_ptr[0x0000+ty*tiles_x+tx];
            uint8_t* tile_data=&mem_ptr[0x1000+tile_index*32];
            for(int py=0;py<tile_size;py++){
                for(int px=0;px<tile_size;px++){
                    uint8_t palette_index=get_4bpp_pixel(tile_data,px,py);
                    int screen_x=(tx*tile_size+px-ppu->scroll_x+256)%256;
                    int screen_y=(ty*tile_size+py-ppu->scroll_y+224)%224;
                    uint32_t color=ppu->palette[palette_index];
                    SDL_SetRenderDrawColor(ppu->renderer,(color>>16)&0xFF,(color>>8)&0xFF,color&0xFF,255);
                    SDL_RenderDrawPoint(ppu->renderer,screen_x,screen_y);
                }
            }
        }
    }
    SDL_RenderPresent(ppu->renderer);
}
