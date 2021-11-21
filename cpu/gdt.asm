section .text

global gdt_load

gdt_load:
	push ebp
	mov ebp, esp
	mov ebx, [ebp+8]
	lgdt[ebx]
	pop ebp
	ret
