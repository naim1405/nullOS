section .text
global flush_gdt
flush_gdt:
mov eax, [esp + 4]
lgdt [eax]


mov ax, 0x10
mov ds, ax
mov ss, ax
mov es, ax
mov gs, ax
mov fs, ax


;code segment
jmp 0x08:.jump

.jump:
ret

