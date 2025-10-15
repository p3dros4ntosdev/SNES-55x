#include "cpu.h"
#include <stdio.h>

void cpu_init(CPU* cpu, Memory* mem){
    cpu->A=0;
    cpu->X=0;
    cpu->Y=0;
    cpu->SP=0xFF;
    cpu->P=0x34;
    cpu->PC=0x8000;
    cpu->mem=mem;
}

void cpu_step(CPU* cpu){
    uint8_t opcode=memory_read(cpu->mem,cpu->PC++);
    switch(opcode){
        case 0xA9:
            cpu->A=memory_read(cpu->mem,cpu->PC++);
            break;
        case 0x8D:{
            uint16_t addr=memory_read(cpu->mem,cpu->PC)|(memory_read(cpu->mem,cpu->PC+1)<<8);
            cpu->PC+=2;
            memory_write(cpu->mem,addr,cpu->A);
            break;
        }
        default: break;
    }
}
