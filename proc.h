#define PROC_NUM 20
int sched_marker;	

unsigned int cur_pid;

struct interrupt_stack{		
	unsigned int edi;	// set up by pusha
	unsigned int esi;
	unsigned int ebp;
	unsigned int oesp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;

	unsigned int eip;	// this line below is setup by the x86 hardware
	unsigned short cs;
	unsigned short padding;
	unsigned int eflags;
	unsigned int esp;
	unsigned short ss;
	unsigned short padding2;
}; 

struct context{
	unsigned int edi;
	unsigned int esi;
	unsigned int ebx;
	unsigned int ebp;
	unsigned int eip;
};

struct proc{
	struct context *context;	// this address will be accessed through assembly code, place it in the first position
	unsigned pid;	
	long timer;	
	unsigned int * kstack;		// points to the base address of this process's kernel stack
	//struct context context;
	struct interrupt_stack *istack;	// interrupt_stack is what the kernel stack looks like when an interrupt occors(without error code);
					// this pointer is set up by fork
	unsigned int * pdt;		// page directory table
	int state;			// state of this process
	char name[8];			// name of this process
};

struct proc proc_table[PROC_NUM];

struct proc *cur_proc;		// current running process
int flag_cur_proc;

#define SLEEPING	1
#define	RUNNING		2
#define UNUSED		0
#define READY		3
#define CREATING	4

struct proc* alloc_proc(void);

int free_proc(struct proc *);

unsigned int getpid(void);

int fork(void);

int exec(char *path);

void switch_task(void);

void set_interrupt_stack(struct proc *);

extern void switch_task_s(void);
extern void switch_context(unsigned int addr);


