#include"interrupt.h"
#include"sys_call.h"

int Install_Id(void* addr,unsigned int gdt_sel,unsigned char flags,unsigned int Num)
{
	if(Num>Num_of_Ints-1)
		return -1;
	if(!addr)
		return -1;

	unsigned long base=(unsigned long)addr;
	
	IDT[Num].base_low=base&0xffff;
	
	IDT[Num].base_high=(base>>16)&0xffff;
	
	IDT[Num].gdt_sel=gdt_sel;

	IDT[Num].reserved=0;

	IDT[Num].flags=flags;

	return 1;
}

void Init_Idtr(void)
{
	idtr.limit=sizeof(struct Int_Desp)*(Num_of_Ints-1);
	idtr.base=(unsigned int)IDT;
	Load_Idtr(idtr);
}

void default_handler(void)	// this is the default c handler, get called from int_handler
{
	//print_char_c('a');	
}

void Init_IDT(void) 
{
	int i;
	for(i=0;i<Num_of_Ints;i++)
	{
		Install_Id(int_handler,gdt_code_sel,flag_32bit|flag_present,i);		
	}
}

void install_sys_call(void)
{
	Install_Id(sys_call_s,gdt_code_sel,flag_32bit|flag_present|flag_ring3,0x80);
}

void Init_intr(void)
{	
	set_8259a();
	set_8253();
	Init_IDT();
	Init_Idtr();
	install_sys_call();
	Enable_Int();
}
