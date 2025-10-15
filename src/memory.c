#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Memory* mem_global=NULL;

void memory_init(void){
    if(mem_global) return;
    mem_global=malloc(sizeof(Memory));
    memset(mem_global,0,sizeof(Memory));
}

void memory_shutdown(void){
    if(mem_global) free(mem_global);
    mem_global=NULL;
}

uint8_t memory_read(Memory* mem,uint32_t addr){
    addr&=0xFFFFFFu;
    return mem->ram[addr%0x2000];
}

void memory_write(Memory* mem,uint32_t addr,uint8_t v){
    addr&=0xFFFFFFu;
    mem->ram[addr%0x2000]=v;
}

void map_rom_chunk(uint32_t dest_addr,const uint8_t* data,size_t len){
    if(!mem_global) return;
    if(dest_addr+len>sizeof(mem_global->rom)) len=sizeof(mem_global->rom)-dest_addr;
    memcpy(&mem_global->rom[dest_addr],data,len);
}

void load_sram_file(const char* path){
    FILE* f=fopen(path,"rb");
    if(!f) return;
    fread(mem_global->ram,1,0x2000,f);
    fclose(f);
}

void save_sram_file(const char* path){
    FILE* f=fopen(path,"wb");
    if(!f) return;
    fwrite(mem_global->ram,1,0x2000,f);
    fclose(f);
}
