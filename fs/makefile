objects=drivers.o driver_s.o file.o sbc.o main.o string.o
cflags=-c -fno-stack-protector

fs: $(objects)
	ld -e main -o fs $(objects)

drivers.o: drivers.c drivers.h
	cc $(cflags) -o drivers.o drivers.c

driver_s.o: drivers.s
	nasm -felf -o driver_s.o drivers.s

file.o: file.c sbc.h string.o file.h
	cc $(cflags) -o file.o file.c

sbc.o: sbc.c fs.h drivers.h
	cc $(cflags) -o sbc.o sbc.c

sring.o: sting.c
	cc $(cflags) -o string.o string.c

main.o: main.c drivers.h sbc.h file.h
	cc $(cflags) -o main.o main.c


clean:
	rm $(objects)
