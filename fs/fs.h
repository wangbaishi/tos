// definition of disk block  inode structure  superblock structure

#define BLOCK_SIZE 512
#define ILIST_START 2		// there is no 0th block, the first block is allocated to the sblock
#define INODE_SIZE (sizeof(Inode))
#define INODES_P_BLOCK (BLOCK_SIZE/INODE_SIZE)
#define DATA_BEGAIN 20
#define ILIST_SIZE 60
#define FS_SIZE 1000

typedef struct{
	char b[BLOCK_SIZE];  	// each block contains 512 bytes
}Block;

typedef struct{
	int type;      			// type of file
	int number;   			// each inode has an unique number   
	int nlink;    			// how many links refered to this inode
	unsigned int size;     		// the size of this file in byte
	int ctime[2]; 			// creat time
	int vtime[2]; 			// last visited time
	unsigned int addr[12]; 		// address
}Inode;

typedef struct{		// sizeof(dir)==16
	int inum;	// so a block of 512 byte may contain 32 dirs
	int mode;
	char name[8];
}dir;

typedef struct{
	unsigned int ilistsize;   	// size in blocks of I list
	unsigned int fsyssize;    	// size in blocks of filesystem
	unsigned int freeblocks;  	// free blocks in the freeb
	unsigned int freeb[100];
	unsigned int freeinodes;  	// free inodes in the freei
	unsigned char freei[60];	// bitmap of the usable inodes
	unsigned int vtime[2];   	// last visited time
}Superblock;

Superblock sblock;
	

