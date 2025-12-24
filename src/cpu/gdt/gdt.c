#include "cpu/gdt/gdt.h"
#include "kernel/types.h"

struct gdt_entry{
	uint16_t 	limit_lower;
	uint16_t 	base_lower;
	uint8_t		base_middle;
	uint8_t		access;
	uint8_t		flag_limit;
	uint8_t		base_high;

} __attribute__ ((packed));

struct gdt_pointer {
	uint16_t size;
	uint32_t start;
} __attribute__ ((packed));

#define GDT_COUNT 3
struct gdt_entry gdt[GDT_COUNT ];
struct gdt_pointer gdtp;

void insert_gdt_entry(uint8_t entry_no,uint32_t base, uint32_t limit,uint8_t access, uint8_t flag){
	gdt[entry_no].base_lower= (base & 0xFFFF);
	gdt[entry_no].base_middle = ((base >> 16) & 0xFF);
	gdt[entry_no].base_high = ((base >> 24) & 0xFF);
	gdt[entry_no].access = access;
	gdt[entry_no].flag_limit = ((limit >> 16) & 0x0F);
	gdt[entry_no].flag_limit = (flag << 4) | gdt[entry_no].flag_limit;
	gdt[entry_no].limit_lower = (limit & 0xFFFF);
}

void setup_gdt(){
	insert_gdt_entry(0, 0, 0, 0, 0);
	insert_gdt_entry(1, 0x00000000,0xFFFFFFFF, 0x9A, 0xCF);
	insert_gdt_entry(2, 0x00000000,0xFFFFFFFF, 0x92, 0xCF);

	gdtp.start = (uint32_t) &gdt;
	gdtp.size = (sizeof(struct gdt_entry) * GDT_COUNT) - 1;

	flush_gdt((uint32_t)&gdtp);
}

