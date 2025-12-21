#include "serial.h"

void kmain(){
	serial_init();
	putc('H');
	putc('I');
	putc('!');
	putc('\n');

	printf("HELLO WORLD!");

}
