#include "lib/port_io.h"
static const unsigned char scancode_to_ascii[128] = {
    0,   27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,   'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,   '\\','z','x','c','v','b','n','m',',','.','/',
    0,   '*',
    0,   ' ',
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,
};

char scancode_to_char(unsigned char scancode)
{
    if (scancode & 0x80) {
        // Key release (break code)
        return 0;
    }
    return scancode_to_ascii[scancode];
}

char read_keyboard(){
unsigned char c = inb(0x60);
	char ascii_c = scancode_to_char(c);
	return  ascii_c;


}
