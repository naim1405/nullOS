#include "lib/port_io.h"
#include "drivers/serial.h"

#define PIC1_CTRL 0x20
#define PIC1_DATA 0x21

#define PIC2_CTRL 0xA0
#define PIC2_DATA 0xA1




void pic_init(){
	// init
	outb(PIC1_CTRL, 0x11);
	outb(PIC2_CTRL, 0x11);
	
	//offset
	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);

	// master slave conn
	outb(PIC1_DATA, 0x04);
	outb(PIC2_DATA, 0x02);

	// mode
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);

	// enable interrupt
	outb(PIC1_DATA, 0xFC);
	outb(PIC2_DATA, 0xFF);
}

void send_pic_ack(int int_num){
	printf("INT CALLED");
	//32-47
	if(int_num < 32 || int_num > 47){
		return;
	}

	if(int_num >= 40){
		outb(PIC2_CTRL, 0x20);
	}

	outb(PIC1_CTRL, 0x20);
}
