section .text

global fush_idt
fush_idt:
mov eax, [esp + 4]
lidt [eax]
ret



%macro no_err_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	push dword 0
	push dword %1 ; interrupt number
	jmp  common_interrupt_handler
%endmacro


%macro err_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	push dword  %1 ; interrupt number
	jmp  common_interrupt_handler
%endmacro


common_interrupt_handler:
extern interrupt_handler
; backup registers
push eax
push ebx
push ecx
push edx
push esi
push edi
push ebp

; call c handler
call interrupt_handler
;restore registers
pop ebp
pop edi
pop esi
pop edx
pop ecx
pop ebx
pop eax

; restore stack pointer
add esp , 8
; return
iret

no_err_code_interrupt_handler 0
no_err_code_interrupt_handler 1
no_err_code_interrupt_handler 2
no_err_code_interrupt_handler 3
no_err_code_interrupt_handler 4
no_err_code_interrupt_handler 5
no_err_code_interrupt_handler 6
no_err_code_interrupt_handler 7
no_err_code_interrupt_handler 9
no_err_code_interrupt_handler 15
no_err_code_interrupt_handler 16
no_err_code_interrupt_handler 18
no_err_code_interrupt_handler 19
no_err_code_interrupt_handler 20
no_err_code_interrupt_handler 21
no_err_code_interrupt_handler 22
no_err_code_interrupt_handler 23
no_err_code_interrupt_handler 24
no_err_code_interrupt_handler 25
no_err_code_interrupt_handler 26
no_err_code_interrupt_handler 27
no_err_code_interrupt_handler 28
no_err_code_interrupt_handler 29
no_err_code_interrupt_handler 30
no_err_code_interrupt_handler 31


err_code_interrupt_handler 8
err_code_interrupt_handler 10
err_code_interrupt_handler 11
err_code_interrupt_handler 12
err_code_interrupt_handler 13
err_code_interrupt_handler 14
err_code_interrupt_handler 17




