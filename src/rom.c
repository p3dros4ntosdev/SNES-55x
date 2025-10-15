#include "rom.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint8_t* romdata=NULL;
static size_t romsize=0;
static int header_size=0;
static int hirom=0;
static char rompath_buf[512];

static int has_smc_header(size_t filesize){
    return (filesize%1024)==512;
}

static void detect_lorom_hirom(void){
    hirom=0;
    size_t lor_off=0x7FC0;
    size_t hir_off=0xFFC0;
    int lor_ok=0,hir_ok=0;
    if(romsize>lor_off+0x20){
        uint16_t sum=romdata[lor_off+0x1E]|(romdata[lor_off+0x1F]<<8);
        uint16_t comp=romdata[lor_off+0x1C]|(romdata[lor_off+0x1D]<<8);
        if((uint16_t)(sum+comp)==0xFFFF) lor_ok=1;
    }
    if(romsize>hir_off+0x20){
        uint16_t sum=romdata[hir_off+0x1E]|(romdata[hir_off+0x1F]<<8);
        uint16_t comp=romdata[hir_off+0x1C]|(romdata[hir_off+0x1D]<<8);
        if((uint16_t)(sum+comp)==0xFFFF) hir_ok=1;
    }
    if(hir_ok && !lor_ok) hirom=1;
    else hirom=0;
}

int rom_load(const char* path){
    FILE* f=fopen(path,"rb");
    if(!f) return -1;
    fseek(f,0,SEEK_END);
    long fsize=ftell(f);
    fseek(f,0,SEEK_SET);
    if(fsize<=0){fclose(f);return -1;}
    header_size=has_smc_header((size_t)fsize)?512:0;
    romsize=(size_t)fsize-header_size;
    romdata=malloc(romsize);
    if(!romdata){fclose(f);return -1;}
    if(header_size) fseek(f,header_size,SEEK_SET);
    fread(romdata,1,romsize,f);
    fclose(f);
    strncpy(rompath_buf,path,sizeof(rompath_buf)-1);
    rompath_buf[sizeof(rompath_buf)-1]='\0';
    detect_lorom_hirom();
    return 0;
}

void rom_unload(void){
    if(romdata) free(romdata);
    romdata=NULL;
    romsize=0;
}

void rom_map_into_memory(void){
    if(!romdata) return;
    size_t pos=0;
    if(!hirom){
        for(int bank=0;bank<=0x7D&&pos<romsize;bank++){
            size_t tocopy=(romsize-pos)>=0x8000?0x8000:(romsize-pos);
            map_rom_chunk(bank<<16|0x8000,&romdata[pos],tocopy);
            pos+=tocopy;
        }
        for(int b=0x80;b<=0xFF;b++){
            int srcb=b&0x7F;
            for(uint32_t i=0;i<0x8000;i++){
                uint8_t v=memory_read(mem_global,(srcb<<16)|0x8000+i);
                memory_write(mem_global,(b<<16)|0x8000+i,v);
            }
        }
    } else {
        for(int bank=0xC0;bank<=0xFF&&pos<romsize;bank++){
            size_t tocopy=(romsize-pos)>=0x10000?0x10000:(romsize-pos);
            map_rom_chunk(bank<<16|0x0000,&romdata[pos],tocopy);
            pos+=tocopy;
        }
        for(int b=0;b<=0x3F;b++){
            for(uint32_t i=0;i<0x10000;i++){
                uint8_t v=memory_read(mem_global,((b+0xC0)<<16)|0x0000+i);
                memory_write(mem_global,(b<<16)|0x0000+i,v);
            }
        }
    }
}

const char* rom_get_path(void){return rompath_buf;}
int rom_is_hirom(void){return hirom;}
size_t rom_size_bytes(void){return romsize;}
