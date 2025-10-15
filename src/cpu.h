#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "memory.h"

typedef struct {
    uint8_t A;
    uint8_t X,Y;
    uint8_t P;
    uint8_t SP;
    uint16_t PC;
    Memory* mem;
} CPU;

void cpu_init(CPU* cpu, Memory* mem);
void cpu_step(CPU* cpu);

#endif
