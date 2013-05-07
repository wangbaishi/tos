global goto_user

goto_user:
	cli
	mov ax,0x23
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	push 0x23
	push 0x800ff0	;stack pointer
	pushf		;eflags
	pop eax
	or eax,0x200	;enable interrupt
	push eax
	push 0x1b
	mov eax,0x800000	;eip
	push eax
	iretd

