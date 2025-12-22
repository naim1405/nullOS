#ifndef GDT_H
#define GDT_H

#include "types.h"

void flush_gdt(uint32_t gdtp);
void setup_gdt();


#endif // !GDT_H
