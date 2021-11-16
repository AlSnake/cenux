section .text

extern _irq_handler

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; IRQ STUB
irq_common_stub:
	; Save CPU state
	pusha

	; Save the data segment descriptor
	mov ax, ds
	push eax

	; Load the kernel data segment descriptor
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	cld
	call _irq_handler

	; Restore Data segment descriptor
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8 ; Cleanup pushed error code and pushed ISQ No

	sti
	iret

; IRQ HANDLERS
_irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

_irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

_irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

_irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

_irq4:
	cli	
	push byte 0
	push byte 36
	jmp irq_common_stub

_irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

_irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub
	
_irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

_irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

_irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

_irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

_irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

_irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

_irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

_irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

_irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub
