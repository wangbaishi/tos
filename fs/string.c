// this file implemented some utilities for string manipulation

void str_cpy(char *dest,char *source)
{
	int i=0;
	while(source[i]!='\0'){
		dest[i]=source[i];
		i++;
	}
	dest[i]='\0';
}

int str_cmp(char *a,char *b)
{
	int i=0;
	while(a[i]!='\0'&&b[i]!='\0'){
		if(a[i]==b[i])
			i++;
			continue;
		if(a[i]>b[i])
			return 1;
		else
			return -1;
	}
	if(a[i]!='\0')
		return 1
	if(b[i]!='\0')
		return -1
	return 0;
}

int str_len(char *str)
{
	int i=0;
	while(str[i]!='\0')
		i++;
	}
	return i;
}
