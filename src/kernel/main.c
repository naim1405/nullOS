#include "drivers/serial.h"
#include "cpu/gdt/gdt.h"
#include "cpu/interrupt/idt.h"


void kmain(){
	setup_gdt();
	serial_init();
	setup_idt();
	__asm__ volatile("sti");
	printf("========================================\n");
	printf("Hello from NullOS!\n");
	printf("========================================\n");

	while(1){
		char c = getc();
		if(c == '\r' || c == '\n'){
			putc('\n');
		}
		else{
		putc(c);

		}
	}

}
