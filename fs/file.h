#include"drivers.h"
#include"sbc.h"
dir deread(Inode i,int dnum);

void dewrite(Inode i,int dnum,dir d);

void dedel(Inode i,int dnum);

void creat(char *p,int mode);

char reada(Inode i,long offset);

void writea(Inode ino,long offset,char d);

void fdelete(Inode ino);

unsigned int bmap(Inode i,long offset);
