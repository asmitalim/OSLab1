all: gbfile-write gbfile-read
	echo "Building files"

gb1: 
	dd if=/dev/zero of=gb1.bin bs=64M count=16

gbfile-write: gbfile-write.o
	gcc -o gbfile-write gbfile-write.o

gbfile-read: gbfile-read.o
	gcc -o gbfile-read gbfile-read.o

.c.o:
	gcc -c $<

run:
	./gbfile-write
	./gbfile-read

clean: 
	rm -f *.o 
	rm -rf gbfile-write
	rm -rf gbfile-read
