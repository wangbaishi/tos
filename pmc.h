#define PAGE_SIZE 4096
#define MAX_PG_NUM 32
#define MAX_PMEM 0X1000 // unit: pages
#define MAX_MEM 128	// size of physical mem the kernel can manage
			// 128*sizeof(unsigned int)*PAGE_SIZE
int debug_pmc;

struct kpage{
	unsigned char page[PAGE_SIZE];
};

struct kpage k_mem[MAX_PG_NUM]__attribute__((aligned(PAGE_SIZE)));		

extern int find_free(unsigned int);

unsigned int u_map[MAX_MEM];	

unsigned int map;

unsigned int *kalloc(void);		//allocate a page size memory in kernel space

int kfree(unsigned int *page);		//free a page size memory in kernel space 

unsigned int *alloc_page(void);		//allocate a page size memory in user space

int free_page(unsigned int *page);	//free a page size memory in user space

void zero_page(unsigned int *page);

void mem_init(void);			//initiate all required data structures


