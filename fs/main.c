#include"file.h"
#include"string.h"

void main()
{
	char deng='A',deng1;
	long loc;
	int i;
	char c;
	Inode tempi;
	Block tempb;
	dir tempd,tempdb;
	int test[200];

	format();
	Bread(1,(char *)&tempb);
	copyd(tempb.b,0,(char *)&sblock.ilistsize,sizeof(Superblock));
	tempi=Iread(1);
	tempd.inum=1;
	tempd.mode=2;
	str_cpy(tempd.name,".");
	dewrite(tempi,1,tempd);
	tempi=Iread(1);
	str_cpy(tempd.name,"..");
	dewrite(tempi,2,tempd);

	//creat("./abc.txt",1);
	//creat("./def",2);
		
	/*for(i=0;i<200;i++)
	{
		test[i]=balloc();
		printf("%d ",test[i]);
	}
	putchar('\n');

	for(i=0;i<200;i++)
	{
		freeb(test[i]);
	}

	for(i=0;i<200;i++)
	{
		test[i]=balloc();
		printf("%d ",test[i]);
	}
	putchar('\n');*/

	tempi=Iread(2);
	for(i=1;i<=400;i++)
	{
		
		//loc=random(5000)+1;
		writea(tempi,loc,deng);
		tempi=Iread(2);
		deng1=reada(tempi,loc);
		if(deng!=deng1)
		{
			//printf("deng\n");
		}
		else
		{
			//printf("%c ",deng);
		}
		deng++;
		if(deng=='Z'+1)
			deng='A';
	}
	/*tempi=Iread(2);
	for(i=1;i<10000L;i+=500)
	{
		c=reada(tempi,i);

		printf("%c ",c);
	}
	
	/*c=reada(tempi,123);
	
	fdelete(tempi);
	tempi=Iread(2);
	tempi=Iread(1);
	for(i=1;i<=10;i++)
	{
		tempd=deread(tempi,i);
		if(tempd.inum!=0)
		{
			printf("%d %d %s\n",tempd.inum,tempd.mode,tempd.name);
		}
	}
	//tempi=Iread(3);
	//tempd=deread(tempi,1);
	//printf("%d %d %s\n",tempd.inum,tempd.mode,tempd.name);*/


	//getchar();
}
