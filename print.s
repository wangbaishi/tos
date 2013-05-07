global print_char,set_cursor,clr_screen,scroll

print_char:
	push ebp
	push edi
	mov ebp,esp
	mov edi,0xb8000
	mov eax,[ebp+12]
	add edi,eax
	mov eax,[ebp+16]	;attr
	mov [edi+1],al
	mov eax,[ebp+20]	;char 
	mov [edi],al
	pop edi
	pop ebp
	ret

clr_screen:
	push edi
	mov edi,0xb8000
	mov ecx,4000
loop1:
	mov [edi],byte 0
	inc edi
	dec ecx
	jnz loop1
	pop edi
	ret

set_cursor:
	push ebp
	push ebx
	mov ebp,esp
	mov ebx,[ebp+12]
	mov al,0x0f 	;low byte
	mov dx,0x03d4
	out dx,al

	mov al,bl
	mov dx,0x03d5
	out dx,al

	mov al,0x0e	;high byte
	mov dx,0x03d4
	out dx,al

	mov al,bh
	mov dx,0x03d5
	out dx,al
	
	pop ebx
	pop ebp
	ret

scroll:
	push edi
	push ecx
	push eax
	mov edi,0xb8000
	mov ecx,1920
loop2:	mov ax,[edi+160]
	mov [edi],ax
	inc edi
	inc edi
	dec ecx
	jnz loop2
	mov cx,80
	xor eax,eax
loop3: 	mov [edi],ax
	inc edi
	inc edi
	dec cx
	jnz loop3
	pop eax
	pop ecx
	pop edi
	ret
