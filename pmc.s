global find_free

find_free:
	push ebp
	push ebx
	mov ebp,esp
	mov ebx,[ebp+12]
	bsr eax,ebx
	jz not_found
	jmp return
not_found:
	mov eax,-1
return: pop ebx
	pop ebp
	ret
