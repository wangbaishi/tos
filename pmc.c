#include"pmc.h"

void zero_page(unsigned int *page)
{
	int i;
	for(i=0;i<1024;i++)
	{
		page[i]=0;
	}
}

unsigned int *kalloc(void)
{
	int i;
	unsigned int mask=0x80000000,*page;
	debug_pmc++;
	i=find_free(map);
	if(i==-1)
	{
		return 0;
	}
	i=31-i;
	mask>>=i;
	mask=~mask;
	map=map&mask;
	page=(unsigned int *)(&(k_mem[i]));
	zero_page(page);
	return(page);
	
}

int kfree(unsigned int *page)
{
	int i,j=0x80000000;
	struct kpage *temp;
	temp=(struct kpage *)page;
	i=temp-k_mem;
	j=j>>i;
	map|=j;
	return 1;
}

int free_page(unsigned int * page)
{
	unsigned int addr=(unsigned int)page,mask=0x80000000;
	int i,off_map,off_mapi;
	addr-=0x400000;
	off_map=addr/PAGE_SIZE;
	i=off_map/sizeof(unsigned int);
	off_mapi=off_map-i*sizeof(unsigned int);
	mask>>=off_mapi;
	u_map[i]|=mask;
	return 1;
}

unsigned int* alloc_page(void)
{
	int i,j,page_num,flag=-1;
	unsigned int addr,mask=0x80000000;
	for(i=0;i<MAX_MEM;i++)
	{
		j=find_free(u_map[i]);
		if(j!=-1)
		{
			j=31-j;
			page_num=i*32+j;
			flag=1;
			mask>>=j;
			mask=~mask;
			u_map[i]=u_map[i]&mask;
			break;
		}
	}
	if(flag==-1)
		return 0;
	addr=page_num*PAGE_SIZE+0x400000;
	//zero_page((unsigned int*)addr);	// can't access this page at this moment 
	return((unsigned int *)addr);
}

unsigned int mem_test(void)	// test how much memory this computer have
{
	unsigned int init_pos=0x400000,step=0x1000,flag=1,count=0;
	unsigned int *data;	
	while(flag)
	{
		data=(unsigned int *)init_pos;
		*data=1234;
		if(*data!=1234)
		{
			flag=0;
			break;
		}
		count++;
		init_pos+=step;
	}
	return count;
}

int test_umem(void)
{
	unsigned int i,*addr_arr[128];
	for(i=0;i<128;i++)
	{
		//print_char_c('!');
		//print_num(i);
		//print_char_c('!');
		addr_arr[i]=alloc_page();
		//free_page(addr[i]);
		//free_page(addr);
	}
	for(i=0;i<128;i++)
	{
		free_page(addr_arr[i]);
	}
	print_num(u_map[2]);
	for(;;);
	
}

int test_kmem(void)
{
	int i;
	unsigned int *temp[32];
	for(i=0;i<32;i++)
	{
		print_char_c('+');
		temp[i]=kalloc();
		if(i>0)
			print_num(temp[i]-temp[i-1]);		
	}
	for(i=0;i<32;i++)
	{
		kfree(temp[i]);
	}
	for(i=0;i<32;i++)
	{
		print_char_c('+');
		temp[i]=kalloc();
		print_char_c('+');
		print_num((unsigned int)temp[i]);		
	}
}
	
void mem_init(void)
{
	unsigned int i,count=0;
	map=0xffffffff;
	//test_kmem();
	
	//count=mem_test();
	if(count>MAX_PMEM)
		count=MAX_PMEM;
	for(i=0;i<MAX_MEM;i++)
	{
		u_map[i]=0xffffffff;
	}	
	//test_umem();
}
