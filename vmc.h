#define Items_in_PDT 1024	// 1024 page tables in a page dirictory table
#define Items_in_PT 1024	// 1024 page table entries in a table
#define PAGE_SIZE 4096
#define KSTACK_SIZE 4096

#define PTE_present 0x1
#define PTE_writable 0x2
#define PTE_user 0x4
#define PTE_accessed 0x10
#define PTE_dirty 0x20
#define PTE_frame 0xfffff000

#define PDE_present 0x1
#define PDE_writable 0x2
#define PDE_user 0x4
#define PDE_write_through 0x8
#define PDE_catch 0x10
#define PDE_accessed 0x20
#define PDE_super_page 0x80
#define PDE_frame 0xfffff000

#define PTE unsigned int
#define PDE unsigned int

#define MAX_DESCRIPTORS 6

int debug;

struct TSS{
	unsigned int link;
	unsigned int esp0;	// ring 0 stack pointer
	unsigned short ss0;	// ring 0 stack segment
	unsigned short padding0; // everything below is unused
	unsigned int esp1;
	unsigned short ss1;
	unsigned short padding1;
	unsigned int esp2;
	unsigned short ss2;
	unsigned short padding2;
	unsigned int cr3;
	unsigned int eip;
	unsigned int eflags;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	unsigned int ldt;
	unsigned int trap;
	unsigned int iomap_base;
}__attribute__((packed));

struct gdt_descriptor{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_mid;
	unsigned char access;
	unsigned char granu;
	unsigned char base_high;
}__attribute__((packed));

struct gdtr{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));

struct gdt_descriptor gdt[MAX_DESCRIPTORS];

struct gdtr gdtreg;

struct TSS tss;

unsigned int temp_map[32];	// bitmap used to manage temporary mapping

unsigned int *kernel_pdt;	// pde points to the page directory table the kernel uses when the system is initiating itself.

extern void enable_paging(unsigned int);
extern void load_gdtr(struct gdtr);
extern void load_tr(void);

void setup_tss(unsigned int *esp);

// functions to be called during creation of a process to set correct memory mapping
// map kernel space into a process's address space
void set_kmap(unsigned int* pdt);	
// map a frame into a process's address space
void map_frame(unsigned int vaddr,unsigned int p_page_addr,unsigned int *pdt);

void set_frame(unsigned int *a,unsigned int frame_num);

unsigned int *temporary_map(unsigned int * page);

void drop_temporary_map(unsigned int *page);

