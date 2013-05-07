global enable_paging,load_gdtr,load_tr

enable_paging:
	push ebp
	mov ebp,esp
	mov eax,[ebp+8]
	mov cr3,eax
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	pop ebp
	ret

load_gdtr:
	push ebp
	mov ebp,esp
	lgdt [ebp+8]
	jmp 08h:complete_flush
complete_flush:
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	pop ebp
	ret

load_tr:
	mov ax,0x2b
	ltr ax
	ret
