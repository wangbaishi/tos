bits 16
extern bootmain
[section .text]
global _start
_start:
	cli
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ax,message
	mov bp,ax
	mov cx,11
	mov ax,1301h
	mov bx,000ch
	mov dl,0
	int 10h		

seta20_1:	
	in al,64h
	test al,2
	jnz seta20_1

	mov al,0xd1
	out 64h,al
seta20_2:
	in al,64h
	test al,2
	jnz seta20_2
	mov al,0xdf
	out 0x60,al

	lgdt [toc]
	
	mov eax,cr0	;enter protected mode
	or eax,1
	mov cr0,eax	

	jmp 08h:pmode
	
bits 32
pmode:
	mov ax,0x10	;initialize seg register
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov esp,90000h

	call bootmain

	hlt

GDT:
	dd 0	; null descriptor
	dd 0 

	;kernel code descriptor
	dw 0ffffh	;limit low
	dw 0		;base low
	db 0		;base middle
	db 10011010b	;access
	db 11001111b	;granularity
	db 0		;base high
	
	;kernel data descriptor	
	dw 0ffffh	
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0	

End_of_GDT:
toc:
	dw End_of_GDT-GDT-1
	dd GDT
message: db "hello,deng!"
;times 510-($-$$) db 0
;dw 0xaa55
