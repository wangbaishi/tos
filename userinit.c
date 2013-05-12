#include"proc.h"
#include"print.h"
#include"userinit.h"
#include"vmc.h"

// this file contain codes that set up the first process

void str_cpy(char * source,char *dest,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		(*dest)=(*source);
		dest++;
		source++;
	}
}

void copy_binary(unsigned int *addr)
{
	extern char _binary_fp_start[],_binary_fp_end[],_binary_fp_size[];
	char * dest;
	int count;
	count=_binary_fp_end-_binary_fp_start;
	dest=(char *)addr;
	str_cpy(_binary_fp_start,dest,(int)_binary_fp_size);
} 
  
void setup_vm(struct proc * a) 
{ 
	unsigned int *page,temp_addr2;	
	a->pdt=(unsigned int *)kalloc();
	set_kmap(a->pdt);		
	page=(unsigned int *)alloc_page();
	temp_addr2=temporary_map(page);
	map_frame(0x800000,(unsigned int)page,a->pdt);		
	copy_binary((unsigned int *)temp_addr2);
	drop_temporary_map((unsigned int *)temp_addr2);	
}

void switch_addr_space(unsigned int * addr)
{
	unsigned int temp=(unsigned int)addr;
	enable_paging(temp);
}

void user_init()	// code used to creat the first process in this system
{
	struct proc *a;
	a=alloc_proc();
	cur_proc=a;	//set the current process pointer to the first process
	flag_cur_proc=0;	// indicates that the cur_proc is not avaliable
	a->kstack=(unsigned int)((char *)kalloc()+KSTACK_SIZE);
	set_interrupt_stack(a);
	setup_vm(a); 
	setup_tss(a->kstack);
	switch_addr_space(a->pdt);
	flag_cur_proc=1;	// cur_proc is avaliable
	goto_user();
}
