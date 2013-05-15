#include"print.h"
#include"pmc.h"
#include"interrupt.h"
#include"vmc.h" 
#include"userinit.h"

// this is the "true" starting point of this system.
// loaded by bootloader at physical memory 0x100000(1M)

void main()   
{ 
	clr_screen();   // clear the screen

	mem_init();   	// initialise physical memory controller
	
	Init_Vmc();	// initialise virtual memeory controller

	Init_intr();	// initialise interrupt 

	user_init();	// user_init() creats the first process in this system

}  
