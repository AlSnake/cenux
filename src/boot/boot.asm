[ORG 0x7c00]
[BITS 16]

CODE_SEG equ gdt_code - gdt_start ; Selector 0x08 will be our code segment offset
DATA_SEG equ gdt_data - gdt_start ; Selector 0x10 will be our data segment offset

_start:
	cli ; Clear interrupts
	
	; clear registers
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00

	; load gdt
	lgdt[gdt_descriptor]

	; set PE (PROTECTION ENABLED) bit in cr0
	mov eax, cr0
	or al, 1
	mov cr0, eax

	; Far jump to selector 8h (gdt 32-bit code segment offset)
	jmp CODE_SEG:load_pm

gdt_start:
	; offset 0x00
	gdt_null:
		dd 0
		dd 0

	; offset 0x08
	gdt_code:
		dw 0xffff ;16-bits of limit
		dw 0 ; 16-bits of base
		db 0 ; 8-bits of base
		db 10011010b ; 4-bit privilege + 4-bit type flags
		db 11001111b ; 4-bit other flags + 4 bits of limit
		db 0 ; 9 bits of base

	; offset 0x10
	gdt_data:
		dw 0xffff
		dw 0
		db 0
		db 10010010b
		db 11001111b
		db 0
gdt_end:

; GDT Pointer to base and limit
gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; limit
	dd gdt_start ; base

[BITS 32]
load_pm:
	
	; set data segment
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

	; Enable A20 Line to access all memory
	in al, 0x92
	or al, 2
	out 0x92, al

	jmp $

times 510-($-$$) db 0
dw 0xAA55
