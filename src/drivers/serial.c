#include "lib/port_io.h"

#define COM1 0x3F8

void serial_init(void){
	// disable interrupt
	outb(COM1 + 1, 0x00);
	// set baud rate
	outb(COM1+3, 0x80);
	outb(COM1+0, 0x00);
	outb(COM1+0, 0x03);
	// set FIFO
	outb(COM1+2, 0xC7);
	// set MODEM
	outb(COM1+4, 0x03);
	// set data line 
	outb(COM1+3, 0x03);
}


int is_tranmission_empty(int port){
	return (inb(port) & 0x20);
}

void putc(char c){
	while(!is_tranmission_empty(COM1+5));
	outb(COM1, c);
}

void printf(char *str){
	//TODO:
	while(*str){
		putc(*str);
		str++;
	}
}

int is_input_ready(int port){
	return (inb(port) & 0x01);
}

char getc(){
	while(!(is_input_ready(COM1+5)));
	return inb(COM1);
}
