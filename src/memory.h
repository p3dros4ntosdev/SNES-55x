#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t ram[0x2000];
    uint8_t rom[0x800000];
} Memory;

extern Memory* mem_global;

void memory_init(void);
void memory_shutdown(void);
uint8_t memory_read(Memory* mem,uint32_t addr);
void memory_write(Memory* mem,uint32_t addr,uint8_t v);
void map_rom_chunk(uint32_t dest_addr,const uint8_t* data,size_t len);
void load_sram_file(const char* path);
void save_sram_file(const char* path);

#endif
