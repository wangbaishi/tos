global switch_task_s, switch_context
extern switch_task,cur_proc

; creat a struct context structure on the top of the stack
; then call switch_task
switch_task_s:
	push ebp
	push ebx
	push esi
	push edi
	mov eax,cur_proc
	mov ebp,esp
	mov [eax],ebp
	call switch_task
	ret
	
switch_context:		; void switch_context(unsigned int esp);
	mov esp,[esp+4]
	pop edi
	pop esi
	pop ebx
	pop ebp
	ret
