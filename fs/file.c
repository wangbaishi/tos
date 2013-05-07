#include"file.h"
#include"string.h"
dir deread(Inode i,int dnum)
{
	int bnum,inb;
	long logoffset;
	dir tempd;
	Block tempb;
	logoffset=(dnum-1)*16;		
	inb=((dnum-1)%32)*16;
	bnum=bmap(i,logoffset);
	Bread(bnum,(char *)&tempb.b);
	copyd(tempb.b,inb,(char *)&tempd.inum,16);
	return(tempd);
}

void dewrite(Inode i,int dnum,dir d)	// write a dir to a file
{
	long logico;
	char tempc;
	int bnum,inb;
	Block tempb;
	logico=(dnum-1)*16;
	writea(i,logico,tempc);
	i=Iread(i.number);
	bnum=bmap(i,logico);
	inb=(dnum-1)%32*16;
	Bread(bnum,(char *)&tempb.b);
	copyd((char *)&(d.inum),0,&(tempb.b[inb]),sizeof(dir));
	Bwrite(bnum,(char *)&tempb.b);
}

void dedel(Inode i,int dnum)
{
	dir tempd;
	tempd=deread(i,dnum);
	tempd.inum=-1;
	dewrite(i,dnum,tempd);
}

unsigned int bmap(Inode i,long offset)	
{
	unsigned int tempo,tempc,*answer;
	Block tempb;
	tempo=(offset/BLOCK_SIZE)+1;
	if(tempo<=10)
	{
		return(i.addr[tempo-1]);
	}
	offset-=10*BLOCK_SIZE;
	if(offset<=128*512L)			// if offset located in the 11th entry
	{
		tempo=offset/BLOCK_SIZE;
		Bread(i.addr[10],(char *)&tempb.b);
		answer=(unsigned int *)tempb.b;
		return(answer[tempo]);
	}
	else
	{
		offset-=128*512L;
		tempo=offset/(128*512L);
		Bread(i.addr[11],(char *)&tempb.b);
		answer=(unsigned int *)tempb.b;
		tempc=answer[tempo];
		Bread(tempc,(char *)&tempb.b);
		tempo=offset%(128*512L);
		tempo/=512;
		answer=(unsigned int *)tempb.b;
		return(answer[tempo]);
	}
}

void movf(char *p,int i)
{
	int len,k;
	len=str_len(p);
	for(k=i;k<=len;k++)
	{
		p[k-i]=p[k];
	}
}

void getname(char *p,char *name)
{
	int i=0;
	while(p[0]!='/'&&p[0]!='\0')
	{
		name[i]=p[0];
		i++;
		movf(p,1);
	}
	name[i]='\0';
}

int ms(char *p) 
{
	if(p[0]=='/')
	{
		movf(p,1);
		return(1);
	}
	else
	{
		return(0);
	}
}

Inode namei(char *p)		// convert a path name to the corresponding inode
{
	int flag=1,i;
	Inode now;
	dir tempd;
	char n[14];
	now=Iread(1);	// root directory
	while(flag)
	{
		getname(p,n);
		for(i=1;i<=ILIST_SIZE;i++)	// unclear->clear: the maximum number of the folder cann't exceed the number of inodes
		{				// so, set ILIST_SIZE as the upper limit is justified.
			tempd=deread(now,i);
			if(!str_cmp(n,tempd.name))
			{
				now=Iread(tempd.inum);
				break;
			}
		}
		if(!ms(p))
			flag=0;
	}
	return(now);
}

void getlstn(char *p,char *n)
{
	int len,i,j=0,k;
	len=str_len(p);
	for(i=len;i>=0;i--)
	{
		if(p[i]=='/')
			break;
	}
	for(k=i+1;k<=len;k++)
	{
		n[j]=p[k];
		j++;
	}
}

void creat(char *p,int mode)
{
	char name[12];
	Inode tempia,tempib;
	dir tempd;
	int i,k,lenn;
	getlstn(p,name);
	tempia=namei(p);
	tempib=ialloc();
	for(i=1;;i++)
	{
		tempd=deread(tempia,i);
		if(tempd.inum==0)
			break;
	}
	tempd.inum=tempib.number;
	lenn=str_len(name);
	for(k=0;k<=lenn;k++)
	{
		tempd.name[k]=name[k];
	}
	tempd.mode=mode;
	dewrite(tempia,i,tempd);
}

char reada(Inode i,long offset)
{
	Inode tempi;
	int bnum,inb;
	Block tempb;
	char tempc;
	tempi=i;
	bnum=bmap(tempi,offset);
	inb=offset%BLOCK_SIZE;
	Bread(bnum,(char *)&tempb.b);
	copyd(tempb.b,inb,&tempc,1);
	return(tempc);
}

void copydm(char *d,char * dest,int offset)
{
	dest[offset]=*d;
}

void writea(Inode ino,long offset,char d)
{
	Inode tempi;
	unsigned int bnum,inb,tempo,i;
	Block tempb,tempc;
	tempi=ino;
	tempo=offset/BLOCK_SIZE+1;
	if(tempo<=10)
	{
		if(tempi.addr[tempo-1]==0)
		{
			tempi.addr[tempo-1]=balloc();
			Iwrite(tempi.number,tempi);		//save the changes
			Bread(tempi.addr[tempo-1],(char *)&tempb);
			for(i=0;i<512;i++)
			{
				tempb.b[i]=0;
			}
			Bwrite(tempi.addr[tempo-1],(char *)&tempb.b);
		}
		Bread(tempi.addr[tempo-1],(char *)&tempb.b);
		inb=offset%BLOCK_SIZE;
		copydm(&d,(char *)tempb.b,inb);
		Bwrite(tempi.addr[tempo-1],(char *)&tempb.b);
	}
	else
	{
		offset=offset-BLOCK_SIZE*10;
		inb=offset%BLOCK_SIZE;
		if(offset<=128*512L)
		{
			tempo=offset/512;
			if(tempi.addr[10]==0)
			{
				tempi.addr[10]=balloc();
				Iwrite(tempi.number,tempi);
				Bread(tempi.addr[10],(char *)&tempb.b);
				for(i=0;i<512;i++)
				{
					tempb.b[i]=0;
				}
				Bwrite(tempi.addr[10],(char *)&tempb.b);
			}
			Bread(tempi.addr[10],(char *)&tempb.b);
			if(tempb.b[tempo]==0)
			{
				tempb.b[tempo]=balloc();
				Bwrite(tempi.addr[10],(char *)&tempb.b);
				Bread(tempb.b[tempo],(char *)&tempc.b);
				for(i=0;i<512;i++)
				{
					tempc.b[i]=0;
				}
				Bwrite(tempb.b[tempo],(char *)&tempc.b);
			}
			Bread(tempb.b[tempo],(char *)&tempc.b);
			copydm(&d,(char *)(&tempc),inb);
			Bwrite(tempb.b[tempo],(char *)&tempc.b);
		}
		else
		{
			offset-=128*512L;
			tempo=offset/(128*512L);
			if(tempi.addr[11]==0)
			{
				tempi.addr[11]=balloc();
				Iwrite(tempi.number,tempi);
				Bread(tempi.addr[11],(char *)&tempb.b);
				for(i=0;i<512;i++)
				{
					tempb.b[i]=0;
				}
				Bwrite(tempi.addr[11],(char *)&tempb.b);
			}
			Bread(tempi.addr[11],(char *)&tempb.b);
			if(tempb.b[tempo]==0)
			{
				tempb.b[tempo]=balloc();
				Bwrite(tempi.addr[11],(char *)&tempb.b);
				Bread(tempb.b[tempo],(char *)&tempc.b);
				for(i=0;i<512;i++)
				{
					tempc.b[i]=0;
				}
				Bwrite(tempb.b[tempo],(char *)&tempc.b);
			}
			Bread(tempb.b[tempo],(char *)&tempc.b);
			i=tempo;
			tempo=offset%(128*512L);
			tempo=tempo/512;
			if(tempc.b[tempo]==0)
			{
				tempc.b[tempo]=balloc();
				Bwrite(tempb.b[i],(char *)&tempc.b);
				Bread(tempc.b[tempo],(char *)&tempb.b);
				for(i=0;i<512;i++)
				{
					tempb.b[i]=0;
				}
				Bwrite(tempc.b[tempo],(char *)&tempb.b);
			}
			Bread(tempc.b[tempo],(char *)&tempb.b);
			copydm(&d,(char *)tempb.b,inb);
			Bwrite(tempc.b[tempo],(char *)&tempb.b);
		}
	}
}

void fdelete(Inode ino)
{
	int i,tempi,j;
	unsigned int *temp_block,*temp_block2;
	Block tempb,tempba;
	for(i=0;i<10;i++)
	{
		freeb(ino.addr[i]);
	}
	tempi=ino.addr[10];
	Bread(tempi,(char *)&tempb.b);
	temp_block=(unsigned int *)tempb.b;
	for(i=0;i<128;i++)
	{
		freeb(temp_block[i]);
	}
	freeb(tempi);
	tempi=ino.addr[11];
	Bread(tempi,(char *)&tempb.b);
	temp_block=(unsigned int *)tempb.b;
	for(i=0;i<128;i++)
	{
		Bread(temp_block[i],(char *)&tempba.b);
		temp_block2=(unsigned int *)tempba.b;
		for(j=0;j<128;j++)
		{
			freeb(temp_block2[j]);
		}
	}
	freeb(tempi);
	ifree(ino.number);	
}

void format(void)
{
	unsigned int i,k=0,bnum;
	Superblock S;
	Block tempb;
	Inode tempi;
	dir tempd;
	S.ilistsize=ILIST_SIZE;
	S.fsyssize=FS_SIZE;
	S.freeblocks=99;
	S.freeinodes=ILIST_SIZE;
	for(i=99;i>=0;i--)		// initialise freeb list
	{
		S.freeb[i]=DATA_BEGAIN+k;
		k++;
	}
	for(i=DATA_BEGAIN+1;i<=S.fsyssize;i++)
	{
		if((i-DATA_BEGAIN+1)%100==0)
		{
			bnum=i+1;
			Bread(i,(char *)&tempb.b);
			for(k=0;k<100;k++)
			{
				tempb.b[k]=bnum;
				bnum++;
				if(bnum>FS_SIZE)
				{
					break;
				}
			}
			Bwrite(i,(char *)&tempb.b);
		}
	}
	S.freei[0]=1;		// the 0th inode is unused
	copyd((char *)&S.ilistsize,0,(char *)tempb.b,sizeof(Superblock));
	Bwrite(1,(char *)&tempb.b);
	for(i=1;i<=ILIST_SIZE;i++)
	{
		tempi=Iread(i);
		tempi.number=i;
		for(k=0;k<12;k++)
		{
			tempi.addr[k]=0;
		}
		Iwrite(i,tempi);
	}
}	

