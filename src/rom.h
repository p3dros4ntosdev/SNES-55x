#include <stddef.h>
#ifndef ROM_H
#define ROM_H

int rom_load(const char* path);
void rom_unload(void);
void rom_map_into_memory(void);
const char *rom_get_path(void);
int rom_is_hirom(void);
size_t rom_size_bytes(void);

#endif
