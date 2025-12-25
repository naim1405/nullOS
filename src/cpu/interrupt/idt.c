#include "include/kernel/types.h"
#include "idt.h"
#include "pic.h"
#include "drivers/serial.h"
#include "drivers/keyboard.h"

#define IDT_COUNT 256

struct idt_entry{
	uint16_t offset_low ;
	uint16_t segment_selector;
	uint16_t  flags;
	uint16_t offet_high;
} __attribute__((packed));

struct idt_pointer {
	uint16_t size;
	uint32_t base;
} __attribute__((packed));


struct idt_entry idt_table[IDT_COUNT];
struct idt_pointer idtp;


void insert_idt_entry(int idx, uint32_t offset, uint16_t segment, uint16_t flags){
	idt_table[idx].offset_low = offset & 0xFFFF;
	idt_table[idx].offet_high= (offset >> 16) & 0xFFFF;
	idt_table[idx].segment_selector = segment;
	idt_table[idx].flags= flags;
}


void setup_idt(){
	insert_idt_entry(0, (uint32_t) &interrupt_handler_0, 0x08, 0x8E00);
	insert_idt_entry(1, (uint32_t) &interrupt_handler_1, 0x08, 0x8E00);
	insert_idt_entry(2, (uint32_t) &interrupt_handler_2, 0x08, 0x8E00);
	insert_idt_entry(3, (uint32_t) &interrupt_handler_3, 0x08, 0x8E00);
	insert_idt_entry(4, (uint32_t) &interrupt_handler_4, 0x08, 0x8E00);
	insert_idt_entry(5, (uint32_t) &interrupt_handler_5, 0x08, 0x8E00);
	insert_idt_entry(6, (uint32_t) &interrupt_handler_6, 0x08, 0x8E00);
	insert_idt_entry(7, (uint32_t) &interrupt_handler_7, 0x08, 0x8E00);
	insert_idt_entry(8, (uint32_t) &interrupt_handler_8, 0x08, 0x8E00);
	insert_idt_entry(9, (uint32_t) &interrupt_handler_9, 0x08, 0x8E00);
	insert_idt_entry(10, (uint32_t) &interrupt_handler_10, 0x08, 0x8E00);
	insert_idt_entry(11, (uint32_t) &interrupt_handler_11, 0x08, 0x8E00);
	insert_idt_entry(12, (uint32_t) &interrupt_handler_12, 0x08, 0x8E00);
	insert_idt_entry(13, (uint32_t) &interrupt_handler_13, 0x08, 0x8E00);
	insert_idt_entry(14, (uint32_t) &interrupt_handler_14, 0x08, 0x8E00);
	insert_idt_entry(15, (uint32_t) &interrupt_handler_15, 0x08, 0x8E00);
	insert_idt_entry(16, (uint32_t) &interrupt_handler_16, 0x08, 0x8E00);
	insert_idt_entry(17, (uint32_t) &interrupt_handler_17, 0x08, 0x8E00);
	insert_idt_entry(18, (uint32_t) &interrupt_handler_18, 0x08, 0x8E00);
	insert_idt_entry(19, (uint32_t) &interrupt_handler_19, 0x08, 0x8E00);
	insert_idt_entry(20, (uint32_t) &interrupt_handler_20, 0x08, 0x8E00);
	insert_idt_entry(21, (uint32_t) &interrupt_handler_21, 0x08, 0x8E00);
	insert_idt_entry(22, (uint32_t) &interrupt_handler_22, 0x08, 0x8E00);
	insert_idt_entry(23, (uint32_t) &interrupt_handler_23, 0x08, 0x8E00);
	insert_idt_entry(24, (uint32_t) &interrupt_handler_24, 0x08, 0x8E00);
	insert_idt_entry(25, (uint32_t) &interrupt_handler_25, 0x08, 0x8E00);
	insert_idt_entry(26, (uint32_t) &interrupt_handler_26, 0x08, 0x8E00);
	insert_idt_entry(27, (uint32_t) &interrupt_handler_27, 0x08, 0x8E00);
	insert_idt_entry(28, (uint32_t) &interrupt_handler_28, 0x08, 0x8E00);
	insert_idt_entry(29, (uint32_t) &interrupt_handler_29, 0x08, 0x8E00);
	insert_idt_entry(30, (uint32_t) &interrupt_handler_30, 0x08, 0x8E00);
	insert_idt_entry(31, (uint32_t) &interrupt_handler_31, 0x08, 0x8E00);
	insert_idt_entry(32, (uint32_t) &interrupt_handler_32, 0x08, 0x8E00);
	insert_idt_entry(33, (uint32_t) &interrupt_handler_33, 0x08, 0x8E00);

	idtp.base = (uint32_t)&idt_table;
	idtp.size = (sizeof(struct idt_entry) * IDT_COUNT) - 1;

	flush_idt((uint32_t) &idtp); pic_init();
}


struct cpu_struct {
	uint32_t ebp;
	uint32_t edi;
	uint32_t esi;
	uint32_t edx;
	uint32_t ecx;
	uint32_t ebx;
	uint32_t eax;
} __attribute__((packed));

struct stack_struct{
	uint32_t err_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
} __attribute__((packed));

// Common interrupt handler
void interrupt_handler(struct cpu_struct cpu, uint32_t int_num, struct stack_struct stack){
	// handle individual interrupt
	if(int_num == 33){
		// keyboard interrupt
		char kb_scan = read_keyboard();
		putc(kb_scan);
	}
	send_pic_ack(int_num);
}


