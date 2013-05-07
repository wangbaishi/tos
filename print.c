#include"print.h"
void print_char_c(char ch)
{	
	if(ch==0xa)
	{
		Cur_posx=0;
		Cur_posy++;
	}
	else
	{
		print_char((Cur_posx+Col_num*Cur_posy)*2,Char_attrib,ch);
		Cur_posx++;
		if(Cur_posx>=Col_num)
		{
			Cur_posx=0;
			Cur_posy++;
		}
	}
	if(Cur_posy>=Row_num)
	{
		Cur_posy=Row_num-1;
		scroll();
	}
	set_cursor(Cur_posx+Col_num*Cur_posy);
}

void print_str_c(char *str)
{
	unsigned int i=0;
	while(str[i]!='\0')
	{
		print_char_c(str[i]);
		i++;
	}
}

void print_num(unsigned int num)
{
	unsigned int length=0,temp,nagtive=0;
	char str_num[20];
	if(num<0)
	{
		nagtive=1;
		num*=-1;
	}
	do{
		temp=num/10;
		str_num[length]=num-temp*10+'0';
		length++;
		num=num/10;
	}while(num>0);
	if(nagtive==1)
	{
		str_num[length]='-';
		length++;
	}
	while(length>0)
	{
		print_char_c(str_num[--length]);
	}
}
	
