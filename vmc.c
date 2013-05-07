#include"vmc.h"
#include"pmc.h"
#include"print.h"
#include"proc.h"

void set_frame(unsigned int *a,unsigned int frame_num)	// ture frame number
{
	frame_num=frame_num<<12;
	(*a)=(*a)|frame_num;
}

int free_slot(unsigned int addr)
{
	unsigned int mask=0x80000000;
	int i,off_map,off_mapi;
	addr-=0x400000;
	off_map=addr/PAGE_SIZE;
	i=off_map/sizeof(unsigned int);
	off_mapi=off_map-i*sizeof(unsigned int);
	mask>>=off_mapi;
	temp_map[i]|=mask;
	return 1;
}

unsigned int find_free_slot(void)
{
	int i,j,page_num,flag=-1;
	unsigned int addr,mask=0x80000000;
	for(i=0;i<32;i++)
	{
		j=find_free(temp_map[i]);
		if(j!=-1)
		{
			j=31-j;
			page_num=i*32+j;
			flag=1;
			mask>>=j;
			mask=~mask;
			temp_map[i]=temp_map[i]&mask;
			break;
		}
	}
	if(flag==-1)
		return 0;
	addr=page_num*PAGE_SIZE+0x400000;
	return addr;			
}

void drop_temporary_map(unsigned int *slot)
{
	int pdt_offset,pt_offset;
	unsigned int *pdt,*pt,mark;
	//print_str_c("dorp temporary map is called with slot equals: ");
	//print_num((unsigned int)slot);
	//print_char_c('\n');
	pdt=cur_proc->pdt;		// all process's pdt is allocated by kalloc(),so, there is no need to temporary map this address
	free_slot((unsigned int)slot);
	pdt_offset=((unsigned int)slot>>22)&0x3ff;

	//print_str_c("should be 1:");
	//print_num(pdt_offset);
	//print_char_c('\n');	
	
	pt_offset=((unsigned int)slot>>12)&0x3ff;
	pt=(unsigned int *)(pdt[pdt_offset]&0xfffff000);
	mark=~PTE_present;
	pt[pt_offset]&=mark;
}

unsigned int *temporary_map(unsigned int * page)		// creat a temporaryly mapping in current process's page directory table 
{							// return the virtual address through which you can access the p_addr
	unsigned int *temp,*pdt;
	int pdt_offset,pt_offset;
	unsigned int free_slot;
	debug++;
	//print_str_c("temporary_map is called\n");
	if(flag_cur_proc==0)
	{
		pdt=kernel_pdt;
	}
	else{
		pdt=cur_proc->pdt;
	}
	free_slot=find_free_slot();
	pdt_offset=((unsigned int)free_slot>>22)&0x3ff;
	pt_offset=((unsigned int)free_slot>>12)&0x3ff;
	if(!(pdt[pdt_offset]&PDE_present))			
	{							
		pdt[pdt_offset]|=PDE_present;
		temp=kalloc();
		set_frame(&(pdt[pdt_offset]),(unsigned int)temp>>12);
	}	
	else					
	{
		temp=(unsigned int *)(pdt[pdt_offset]&PDE_frame);
	}
	temp[pt_offset]|=PTE_present;		
	temp[pt_offset]|=PTE_writable;
	set_frame(&(temp[pt_offset]),(unsigned int)page>>12);
	//print_str_c("temporary map want to return with slot equals: ");
	//print_num((unsigned int)free_slot);
	//print_char_c('\n');
	return (unsigned int *)free_slot;
}

void set_kmap(unsigned int *pdt)	// this function works exactly as Set_Identity_Map()
{					// except that it works with a process's pdt
	unsigned int *pte,*temp_pte,frame=0;
	int i;
	pte=alloc_page();
	temp_pte=temporary_map(pte);
	//print_str_c("temporary_map returned\n");
	set_frame(&(pdt[0]),(unsigned int)pte>>12); 
	pdt[0]|=PDE_present;
	for(i=0;i<1024;i++)
	{
		set_frame(&(temp_pte[i]),frame);
		temp_pte[i]|=PTE_present;
		temp_pte[i]|=PTE_writable;	
		frame++;
	}
}

void map_frame(unsigned int vaddr,unsigned int p_page_addr,unsigned int *pdt)
{
	int pdt_offset,pt_offset;
	unsigned int *pte;
	pdt_offset=(vaddr>>22)&0x3ff;
	pt_offset=(vaddr>>12)&0x3ff;
	if(!(pdt[pdt_offset]&PDE_present))
	{	
		pte=alloc_page();
		temporary_map(pte);
		pdt[pdt_offset]|=PDE_present;
		pdt[pdt_offset]|=PDE_user;
		pdt[pdt_offset]|=PDE_writable;		
		set_frame(&(pdt[pdt_offset]),((unsigned int)pte)>>12);
	}
	pte=(unsigned int *)(pdt[pdt_offset]&0xfffff000); // address of page table
	if(!(pte[pt_offset]&PTE_present))
	{
		pte[pt_offset]|=PTE_present;
		pte[pt_offset]|=PTE_user;
		pte[pt_offset]|=PTE_writable;
		set_frame(&(pte[pt_offset]),p_page_addr>>12);
	}
}

int Set_Identity_Map(void)
{
	int i;
	unsigned int frame=0;
	kernel_pdt=(unsigned int *)kalloc();
	unsigned int *pt=(unsigned int *)kalloc();
	if(!kernel_pdt||!pt)
		return -1;
	for(i=0;i<1024;i++)
	{
		set_frame(&(pt[i]),frame);
		pt[i]|=PTE_present;
		pt[i]|=PTE_writable;		
		frame++;
	}
	set_frame(&(kernel_pdt[0]),(unsigned int)pt>>12);
	kernel_pdt[0]|=PDE_present;
	return 1;
}	

void set_gdt_descriptor(int i,unsigned int base,unsigned int limit,unsigned char access,unsigned char granu)
{
	gdt[i].limit_low=limit&0xffff;
	gdt[i].base_low=(unsigned short)base&0xffff;
	gdt[i].base_mid=(unsigned char)((base>>16)&0xff);
	gdt[i].access=access;
	gdt[i].granu=granu;
	gdt[i].base_high=(unsigned char)((base>>24)&0xff);
}

void setup_tss(unsigned int *esp)	// set stack pointer in tss
{
	tss.esp0=(unsigned int)esp;
}

void init_tss(void)
{
	unsigned int base=(unsigned int)(&tss);
	unsigned int limit=sizeof(struct TSS);
	tss.ss0=0x10;
	set_gdt_descriptor(5,base,limit,0x89,0x40);
	load_tr();
}

void init_gdt(void)
{
	set_gdt_descriptor(0,0,0,0,0);
	set_gdt_descriptor(1,0,0xffff,0x9a,0xcf);	// kernel code descriptor
	set_gdt_descriptor(2,0,0xffff,0x92,0xcf);	// kernel data descriptor
	set_gdt_descriptor(3,0,0xffff,0xfa,0xcf);	// user code descriptor
	set_gdt_descriptor(4,0,0xffff,0xf2,0xcf);	// user data descriptor	
	gdtreg.limit=sizeof(struct gdt_descriptor)*MAX_DESCRIPTORS-1;
	gdtreg.base=(unsigned int)(gdt);
	load_gdtr(gdtreg);
	init_tss();					// tss descriptor; this function will cause triple fault if 
							// called before load gdtr
}

void Init_Vmc(void)
{
	int i;
	init_gdt();
	Set_Identity_Map();
	//Set_Vm();
	enable_paging((unsigned int)kernel_pdt);
	for(i=0;i<32;i++)
	{
		temp_map[i]=0xffffffff;
	}
}
