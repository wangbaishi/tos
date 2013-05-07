#include"print.h"
#include"pmc.h"
#include"interrupt.h"
#include"vmc.h" 
#include"userinit.h"

void main()   
{ 
	clr_screen();       

	mem_init();   
	
	Init_Vmc();

	Init_intr();

	user_init();
	
	//for(;;);
}  
