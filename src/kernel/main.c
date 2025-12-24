#include "drivers/serial.h"
#include "cpu/gdt/gdt.h"

void kmain(){
	 setup_gdt();

	serial_init();
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
