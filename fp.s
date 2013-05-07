; this is the instructions of the first process.
; it first gets it's pid through system call then "fork"
; after fork return, the father loops, and the child 
; "exec" the shell program.

global abc

abc:	mov eax,0	; 0 is the system call number of get_pid()
	int 0x80
	mov [pid_father],eax 
	mov eax,1	; 1: fork()
	int 0x80
	push eax
	mov eax,0
	int 0x80
	cmp eax,[pid_father]
	jz loop
	mov eax,2	; 2: exec()
	int 0x80	
loop:	
	mov eax,3	; 3: switch()
	int 0x80	
	jmp $     

pid_father:	dd 0

