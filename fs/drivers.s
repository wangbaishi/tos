global outbyte,inbyte,inword,outword

outbyte:		; void outbyte(unsigned int port,unsigned char byte);
	push ebp
	push edx
	mov ebp,esp
	mov edx,[ebp+12];
	mov eax,[ebp+16]
	out dx,al
	pop edx
	pop ebp
	ret

outword:		; void outword(unsigned int port,unsigned short word);
	push ebp
	push edx
	mov ebp,esp
	mov edx,[ebp+12]
	mov eax,[ebp+16]
	out dx,ax
	pop edx
	pop ebp
	ret

inbyte:			; unsigned char inbyte(unsigned int port);
	push ebp
	push edx
	mov ebp,esp
	mov edx,[ebp+12]
	in al,dx
	pop edx
	pop ebp
	ret

inword:			;unsigned short inword(unsigned int port);
	push ebp
	push edx
	mov ebp,esp
	mov edx,[ebp+12]
	in ax,dx
	pop edx
	pop ebp
	ret

