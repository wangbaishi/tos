CC = $(TOOLPREFIX)gcc
CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -m32 #-MD -ggdb -m32 
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
my_os: bootloader kernel
	dd if=/dev/zero of=boot.img bs=512 count=20160	# ten M bytes
	dd if=/dev/zero of=bootsec bs=512 count=1
	dd if=bootloader of=bootsec conv=notrunc	
	python sign.py
	dd if=bootsec of=boot.img bs=512 count=1 conv=notrunc
	dd if=kernel of=boot.img seek=1 conv=notrunc	

bootloader: boot.s bootmain.c
	$(CC) $(CFLAGS) -fno-pic -O -nostdinc -I. -c bootmain.c
	nasm -felf32 -o boot.o boot.s
	ld -m elf_i386 -n -e start -Ttext 0x07c00  boot.o bootmain.o -o bootloader.o
	objdump -S bootloader.o>bootloader.asm
	objcopy -S -O binary -j .text bootloader.o bootloader	

fp: fp.s
	nasm -felf32 -o fp.o fp.s
	ld -m elf_i386 -N -e abc -Ttext 0x800000 -o fp.out fp.o
	objcopy -S -O binary fp.out fp
	#objcopy --input binary --output elf32-i386 --binary-architecture i386 fp.out fp
	objdump -S fp.out>fp.asm

kernel: main.c print.c pmc.c pmc.s vmc.c vmc.s print.s interrupt.c interrupt.s userinit.c userinit.s proc.c sys_call.c sys_call.s proc.s fp
	cc -o main.o -c $(CFLAGS) -fno-pic -nostdinc main.c
	cc -o printc.o -c $(CFLAGS) -nostdinc print.c
	cc -o pmc.o -c $(CFLAGS) -nostdinc pmc.c
	cc -o interrupt.o $(CFLAGS) -nostdinc -c interrupt.c
	cc -o vmc.o -c $(CFLAGS) -nostdinc vmc.c
	cc -o userinit.o -c $(CFLAGS) -nostdinc userinit.c
	cc -o proc.o -c $(CFLAGS) -nostdinc proc.c
	cc -o sys_call.o -c $(CFLAGS) -nostdinc sys_call.c
	nasm -felf32 -o procs.o proc.s
	nasm -felf32 -o sys_calls.o sys_call.s
	nasm -felf32 -o userinits.o userinit.s
	nasm -felf32 -o pmcs.o pmc.s
	nasm -felf32 -o vmcs.o vmc.s
	nasm -felf32 -o interrupts.o interrupt.s
	nasm -felf32 -o prints.o print.s	
	ld -m elf_i386 -M > kernel.map -e main -Ttext 0x100000 -o kernel \
		main.o prints.o printc.o pmc.o pmcs.o interrupt.o interrupts.o vmc.o vmcs.o\
		userinit.o userinits.o proc.o sys_call.o sys_calls.o procs.o -b binary fp
	objdump -S kernel>main.asm
	objdump -t kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym
	
clean:
	rm *.o
	rm kernel.map
	rm kernel.sym
	rm fp
	rm fp.asm
	rm fp.out
	rm kernel
	rm bootsec
	rm bootloader


