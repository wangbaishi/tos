#define Num_of_Ints 256

struct Int_Desp{
	unsigned short base_low;
	unsigned short gdt_sel;
	unsigned char reserved;
	unsigned char flags;
	unsigned short base_high;
}__attribute__((packed));

struct Idtr{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));

struct Int_Desp IDT[Num_of_Ints];
struct Idtr idtr;

extern int sys_call_s(void);
extern void set_8259a(void);
extern void Load_Idtr(struct Idtr address);
extern void Enable_Int(void);
extern void send_eoi(void);
extern void int_handler(void);
extern void set_8253(void);
extern void run(unsigned int);

int Install_Id(void* addr,unsigned int gdt_sel,unsigned char flags,unsigned int Num);

#define gdt_code_sel 0x8
#define flag_32bit 0x0e
#define flag_present 0x80
#define flag_ring3 0x60

