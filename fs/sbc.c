#include"drivers.h"
#include"sbc.h"

Inode Iread(int i)		// read ith Inode. i starts from 0
{
	int bnum,inb;
	Inode tempi;
	Block tempb;
	bnum=i/INODES_P_BLOCK+ILIST_START;		// which block does the Inode reside in
	inb=(i%INODES_P_BLOCK)*INODE_SIZE;		// the offset of the inode in the block
	Bread(bnum,(char *)&tempb.b);
	copyd(tempb.b,inb,(char *)&(tempi.type),INODE_SIZE);
	return(tempi);
}

void Iwrite(int inum,Inode inode)	// write the ith block
{
	Block tempb;
	int bnum,shift;
	bnum=inum/INODES_P_BLOCK+ILIST_START;
	Bread(bnum,(char *)&tempb.b);
	shift=(inum%INODES_P_BLOCK)*INODE_SIZE;
	copyd((char *)&inode.type,0,&(tempb.b[shift]),INODE_SIZE);
	Bwrite(bnum,(char *)&tempb);
}

Inode ialloc(void)	
{
	int i,bnumber,inb,tempaddr;
	Inode tempi;
	sblock.freeinodes--;
	for(i=0;i<ILIST_SIZE;i++){
		if(sblock.freei[i]==0){
			sblock.freei[i]=1;
		}
	}
	return(Iread(i));
}

void ifree(int i)
{
	sblock.freeinodes++;
	sblock.freei[i]=0;
}

int balloc(void)
{
	unsigned int tempbnum,i;
	unsigned int *temp;
	Block tempb;
	if(sblock.freeblocks==0)	//sblock.freeblocks is initialised to 99, points to the last item in the list
	{		
		tempbnum=sblock.freeb[0];
		Bread(tempbnum,(char *)&tempb);
		temp=(unsigned int *)(tempb.b);
		for(i=99;i>=0;i--)
		{
			sblock.freeb[i]=temp[99-i];
		}
		sblock.freeblocks=99;
		return tempbnum;
	}
	else
	{
		tempbnum=sblock.freeb[sblock.freeblocks];
		sblock.freeblocks--;
		return tempbnum;
	}
}

void freeb(int bnumber)
{
	int i;
	unsigned int *temp;
	if(bnumber==0)
	{
		return;
	}
	sblock.freeblocks++;
	if(sblock.freeblocks==100)
	{
		Block tempb;
		Bread(bnumber,(char *)&tempb);
		temp=(unsigned int *)(tempb.b);
		for(i=0;i<=99;i++)
		{
			temp[i]=sblock.freeb[99-i];
		}
		Bwrite(bnumber,(char *)&tempb);
		sblock.freeblocks=0;
		sblock.freeb[0]=bnumber;
	}
	else
	{
		sblock.freeb[sblock.freeblocks]=bnumber;
	}
}
	
