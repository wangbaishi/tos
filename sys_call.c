#include"sys_call.h"
#include"proc.h"

void get_parameter_str(char *str)
{

}

void set_return_parameter(int states)
{
	struct interrupt_stack *is;
	is=cur_proc->istack;
	is->eax=states;
}

// system call handler, there is no use to return any value
// because all registers will be overwrote by "popa".
// only way to return a value is to write the interrupt stack 
// of the calling process.
void sys_call(void)	
{			
	int call_num;	
	int states;
	char par_str[20];
	call_num=get_call_num();
	switch (call_num){
		case 0:
			states=getpid();
			set_return_parameter(states);
			return;
		case 1:
			fork();		// fork already set the return parameter to 1
			return;			
		case 2: 
			get_parameter_str(par_str);
			states=exec(par_str);
			set_return_parameter(states);
			return;
		case 3: 
			switch_task();	// switch_task() will never returns
	}	
}

