MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
MBFLAGS equ (MBALIGN | MEMINFO) ; 0b000011
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)


section .multiboot
align 4
dd MAGIC
dd MBFLAGS
dd CHECKSUM


section .bss
align 16
stack_end:
resb 16384 ; 16 kb reserved for stack
stack_top:


section .text
global _start
_start:
mov esp, stack_top
mov eax, 0x4444

cli ; disable interrupt
.hang: hlt
jmp .hang
.end:
