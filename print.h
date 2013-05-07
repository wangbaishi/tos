#define Col_num 80
#define Row_num 25
#define Char_attrib 14

unsigned int Cur_posx,Cur_posy;

extern void print_char(unsigned int mem_pos,unsigned char attribute,char ch);

extern void set_cursor(unsigned int position);

extern void clr_screen(void);

extern void scroll(void);

void print_char_c(char c);

void print_str_c(char *str);

void print_num(unsigned int num);




