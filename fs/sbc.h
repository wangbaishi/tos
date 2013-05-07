#include"fs.h"
Inode Iread(int i);	// return the ith Inode
 
void Iwrite(int inum,Inode inode);	// write the ith inode back to the ilist

Inode ialloc(void);	// allocate an inode

void ifree(int i);	// free an inode
	
int balloc(void);	// allocate an data block 

void freeb(int bnumber);	// free an data block


