MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
MBFLAGS equ (MBALIGN | MEMINFO) ; 0b000011
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

KERNEL_OFFSET equ 0xC0000000


section .multiboot
align 4
dd MAGIC
dd MBFLAGS
dd CHECKSUM



section .bss

align 4096
page_directory:
resb 4096 ; 1024 entries

page_table:
resb 4096 ; 1024 entries

align 16
stack_end:
resb 16384 ; 16 kb reserved for stack
stack_top:


section .text
global _start
extern kmain
_start:
mov esp, stack_top - KERNEL_OFFSET

; set page directory location in cr3
mov eax, page_directory - KERNEL_OFFSET
mov cr3, eax


; construct page table entries
mov ecx, 0
mov eax, 0
.pt_loop:
	mov ebx, eax
	or ebx, 1
	or ebx, 2
	mov [page_table - KERNEL_OFFSET + ecx*4], ebx

	add eax, 4096
	add ecx, 1
	cmp ecx, 1024
	jne .pt_loop


; map page table to pdt
mov eax, page_table - KERNEL_OFFSET
or eax, 1
or eax, 2
mov [page_directory - KERNEL_OFFSET], eax
mov [page_directory - KERNEL_OFFSET + 768*4], eax



; enable paging
mov ebx, cr0
or ebx, 0x80000000
mov cr0, ebx

; jump to higher half kernel
lea ebx, [higher_half]
jmp ebx


higher_half:
mov esp, stack_top
mov [page_directory - KERNEL_OFFSET], 0
invlpg [0]

cli ; disable interrupt
call kmain ; call c kernel code

.hang: hlt
jmp .hang
.end:
