
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

struct proc{
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

struct context{
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
};

struct proc proc_table[20];

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

void switch_task(void);
