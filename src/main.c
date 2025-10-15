#include "SDL.h"
#include "ppu.h"
#include "cpu.h"
#include "memory.h"
#include "rom.h"

//Lacking Audio PPU And SDL2-3 implementation



int main(int argc,char** argv){
    if(argc<2) return 1;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win=SDL_CreateWindow("SNES",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,512,448,0);
    SDL_Renderer* ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    memory_init();
    rom_load(argv[1]);
    rom_map_into_memory();

    PPU ppu;
    ppu_init(&ppu,mem_global,ren);

    CPU cpu;
    cpu_init(&cpu,mem_global);

    int frame=0;
    int running=1;
    SDL_Event e;
    while(running){
        while(SDL_PollEvent(&e)) if(e.type==SDL_QUIT) running=0;
        cpu_step(&cpu);
        ppu_set_scroll(&ppu,frame%256,frame%224);
        ppu_render_frame(&ppu);
        frame++;
    }

    rom_unload();
    memory_shutdown();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
