// the finctions in this file simulate the drivers in a real system
#include"drivers.h"

extern unsigned char inbyte(unsigned int port);
extern unsigned short inword(unsigned int port);
extern void outbyte(unsigned int port,unsigned char byte); extern void outword(unsigned int port,unsigned short word);

void read_data(char *buffer)
{
	int i;
	unsigned short *temp;
	temp=(unsigned short *)buffer;
	for(i=0;i<256;i++)
	{
		temp[i]=inword(0x1f0);
	}
}

void init_opreation(unsigned int addr,int flag)
{
	unsigned char temp;
	unsigned short count=1;
	outbyte(0x1f6,0x40);
	temp=(unsigned char)(count>>8);
	outbyte(0x1f2,temp);
	temp=(unsigned char)(addr>>24);
	outbyte(0x1f3,temp);
	outbyte(0x1f4,0);
	outbyte(0x1f5,0);
	temp=(unsigned char)count;
	outbyte(0x1f2,temp);
	temp=(unsigned char)addr;
	outbyte(0x1f3,temp);
	temp=(unsigned char)(addr>>8);
	outbyte(0x1f4,temp);
	temp=(unsigned char)(addr>>16);
	outbyte(0x1f5,temp);
	if(flag==0)
	{
		outbyte(0x1f7,0x24);
	}
	else{
		outbyte(0x1f7,0x34);
	}
}

int check_flags(void)
{
	int i;
	unsigned char temp;
	for(i=0;i<4;i++){		// delay
		temp=inbyte(0x1f7);
		
	}
	while(temp&0x80){		// busy
		temp=inbyte(0x1f7);
	}
	if(temp&8){			// ready
		//read_data(buffer);
		return 1;
	}
	while((temp=inbyte(0x1f7))&0x80);	// busy
	if(temp&0x21){				// error
		return -1;
		//print_str_c("error on reading");
	}
}

void Bread(unsigned int addr,char *buffer) 
{
	int flag;
	init_opreation(addr,0);
	flag=check_flags();
	if(flag==1){
		read_data(buffer);
	}
	else{
		//print_str_c("error on reading disk\n");
	}
}

void write_data(char *b)
{
	unsigned short *temp;
	int i,j;
	temp=(unsigned short *)b;
	for(i=0;i<256;i++){
		outword(0x1f0,temp[i]);
		for(j=0;j<10;j++);
	}
	outbyte(0x1f7,0xe7);
}

void Bwrite(unsigned int addr,char *buffer)
{
	int flag;
	init_opreation(addr,1);
	flag=check_flags();
	if(flag==1){
		write_data(buffer);
	}
	else{
		//print_str_c("error on writing disk\n");
	}
}

/*void Bread(int addr,Block *B,int number)
{
	FILE *p;
	int i;
	if(addr==0)
	{
		for(i=0;i<256;i++)
		{
			(*B).b[i]=0;
		}
		return;
	}
	p=fopen("disk.data","rb+");
	fseek(p,(long)(addr-1)*sizeof(Block),0);
	fread(B,sizeof(Block),number,p);
	fclose(p);
}

void Bwrite(int addr,Block *B,int number)
{
	FILE *p;
	p=fopen("disk.data","rb+");
	fseek(p,(long)(addr-1)*sizeof(Block),0);
	fwrite(B,sizeof(Block),number,p);
	fclose(p);
}*/

void copyd(char *source,int offset,char *dest,int count)
{
	int i;
	char *true_source;
	true_source=source+offset;
	for(i=0;i<count;i++){
		*(dest++)=*(true_source++);
	}
}
