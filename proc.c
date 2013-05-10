#include"proc.h"
#include"vmc.h"
#include"interrupt.h"
#include"userinit.h"

struct proc* alloc_proc(void)	//return a proc structure with state set to CREATING
{
	int i;
	for(i=0;i<32;i++)
	{
		if(proc_table[i].state==UNUSED)
		{
			proc_table[i].state=CREATING;
			proc_table[i].pid=cur_pid;
			cur_pid++;
			return(&(proc_table[i]));
		}
	}
	return 0;
}

int free_proc(struct proc * proc)
{
	if(proc->state==UNUSED)
		return -1;
	proc->state=UNUSED;
	return 1;
}


unsigned int getpid(void)
{
	return cur_proc->pid;
}

struct proc *find_ready(void)	
{
	int i;
	if(sched_marker==0){
		sched_marker=1;
	}
	for(i=sched_marker+1;i!=sched_marker;i++){
		if(i==PROC_NUM){
			i=1;
		}
		if(proc_table[i].state==READY){
			sched_marker=(i+1)%PROC_NUM;
			return &proc_table[i];
		}
	}
	return &proc_table[0];	// process 0 is always ready
}

// scan the process table to find a READY process
// and puts it to run.
void switch_task(void)		
{				
	struct proc *a;
	//save_context(cur_proc->context);
	a=find_ready();
	cur_proc=a;
	setup_tss(a->kstack);
	//run((unsigned int)a->istack);
}

void copy_kstack(unsigned int *sp)
{
	unsigned int i,*f_sp;
	f_sp=cur_proc->kstack;
	for(i=0;i<(KSTACK_SIZE/4);i++)
	{
		*(--sp)=*(--f_sp);
	}
}

void copy_page(unsigned int *source,unsigned int *dest)
{
	int i;
	for(i=0;i<(PAGE_SIZE/4);i++)
	{
		*(dest++)=*(source++);
	}
}

void copy_mem(unsigned int *pdt)
{
	unsigned int *p_pdt,*p_pt,*pt,*temp_addr1,*p_page,*page,*temp_addr2,flags;
	int i,j;
	p_pdt=cur_proc->pdt;	// parent pdt
	for(i=2;i<Items_in_PDT;i++)	// item 0 and item 1 are used for kernel pdt
	{
		if(p_pdt[i]&PDE_present)
		{
			//pdt[i]=p_pdt[i];	 // bug, set_frame uses bitwise OR to set the frame
			pdt[i]=0;
			flags=p_pdt[i]&0x0fff;
			pdt[i]=pdt[i]|flags;		// copy flags
			pt=(unsigned int *)alloc_page();
			set_frame(&(pdt[i]),((unsigned int)pt)>>12);
			temp_addr1=temporary_map(pt);
			p_pt=(unsigned int *)(p_pdt[i]&PDE_frame);
			p_pt=temporary_map(p_pt);
			for(j=0;j<Items_in_PT;j++)
			{
				if(p_pt[j]&PTE_present)
				{
					flags=p_pt[j]&0x0fff;
					temp_addr1[j]=0;
					temp_addr1[j]=temp_addr1[j]|flags;					
					//temp_addr1[j]=p_pt[j];
					page=(unsigned int *)alloc_page();
					temp_addr2=temporary_map(page);
					set_frame(&(temp_addr1[j]),((unsigned int)page)>>12);
					p_page=(unsigned int *)(p_pt[j]&PTE_frame);
					p_page=temporary_map(p_page);
					copy_page(p_page,temp_addr2);
					drop_temporary_map(temp_addr2);
					drop_temporary_map(p_page);
				}
			}
			drop_temporary_map(temp_addr1);
			drop_temporary_map(p_pt);
		}
	}
}

void set_interrupt_stack(struct proc *a)
{
	char *sp;
	sp=(char *)(a->kstack);
	a->istack=(unsigned int *)(sp-sizeof(struct interrupt_stack));
	a->istack->eax=1;		// fork return 1
}

void print_interrupt_stack(unsigned int *istack)
{

}

int fork(void)		// creat a copy of the process who called this funtion
{			// return 1 if successed.
	struct proc *a;
	//print_str_c("fork() called\n");
	a=alloc_proc();
	//print_str_c("alloc_proc() returned\n");
	a->kstack=(unsigned int *)((unsigned char *)kalloc()+KSTACK_SIZE);
	//print_str_c("prepare to call copy_kstack()\n");	
	copy_kstack(a->kstack);
	set_interrupt_stack(a);
	//print_interrupt_stack(a->istack);
	//print_str_c("copy_kstack() returned\n");
	a->pdt=(unsigned int *)kalloc(); 
	set_kmap(a->pdt);
	//print_str_c("set_kmap returned\n");
	copy_mem(a->pdt);
	//print_str_c("copy mem returned\n");
	a->state=READY;
	//enable_paging((unsigned int)a->pdt);
	//print_str_c("pdt[2] is: ");
	//print_num(a->pdt[2]);	
	return 1;
}

int exec(char *path)		//replace the content of the text area 
{
	return 1;
}
