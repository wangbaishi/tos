%define ICW_1 0x11				; 00010001 binary. Enables initialization mode and we are sending ICW 4
 
%define PIC_1_CTRL 0x20				; Primary PIC control register
%define PIC_2_CTRL 0xA0				; Secondary PIC control register
 
%define PIC_1_DATA 0x21				; Primary PIC data register
%define PIC_2_DATA 0xA1				; Secondary PIC data register
 
%define IRQ_0	0x20				; IRQs 0-7 mapped to use interrupts 0x20-0x27
%define IRQ_8	0x28				; IRQs 8-15 mapped to use interrupts 0x28-0x36
 
global set_8259a,Load_Idtr,Enable_Int,set_8253,send_eoi,int_handler,sys_call_s,run
extern default_handler,sys_call

set_8259a:
	mov	al, ICW_1
	out	PIC_1_CTRL, al
  
	out	PIC_2_CTRL, al
 
	mov	al, IRQ_0
	out	PIC_1_DATA, al
 
	mov	al, IRQ_8
	out	PIC_2_DATA, al
 
	mov	al, 0x4			; 0x04 => 0100, second bit (IR line 2)
	out	PIC_1_DATA, al		; write to data register of primary PIC
 
	mov	al, 0x2			; 010=> IR line 2
	out	PIC_2_DATA, al		; write to data register of secondary PIC
 
	mov	al, 1			; bit 0 enables 80x86 mode
 
	out	PIC_1_DATA, al
	out	PIC_2_DATA, al
 
	mov	al, 0
	out	PIC_1_DATA, al
	out	PIC_2_DATA, al
	
	ret

Load_Idtr:
	push ebp
	mov ebp,esp
	lidt [ebp+8]
	pop ebp
	ret

Enable_Int:
	sti
	ret

set_8253:
	mov dx,1193180/100	; interrupt rate:100HZ
	mov al,110110b
	out 0x43,al
	mov ax,dx
	out 0x40,al
	xchg ah,al
	out 0x40,al
	ret

send_eoi:
	push eax
	mov al,0x20
	out 0x20,al
	out 0xA0,al
	pop eax
	ret

int_handler:
	pushad
	call default_handler
	call send_eoi
	sti		; do I really need to sti here?
	popad
	iret

sys_call_s:
	pushad
	call sys_call
	popad
	iret

run:
	mov ebp,[esp+4]
	mov esp,ebp
	popa
	iret
