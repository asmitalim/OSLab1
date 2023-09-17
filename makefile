all: gbfile-write gbfile-read gbfile-mmap-read gbfile-mmap-write
	echo "Building files"

gb1: 
	dd if=/dev/zero of=gb1.bin bs=64M count=16

gbfile-write: gbfile-write.o
	gcc -o gbfile-write gbfile-write.o

gbfile-read: gbfile-read.o
	gcc -o gbfile-read gbfile-read.o

gbfile-mmap-read : gbfile-mmap-read.o
	gcc -o gbfile-mmap-read gbfile-mmap-read.o

gbfile-mmap-write : gbfile-mmap-write.o
	gcc -o gbfile-mmap-write gbfile-mmap-write.o

.c.o:
	gcc -c $<

run:
	./gbfile-write
	./gbfile-read

clean: 
	rm -f *.o 
	rm -rf gbfile-write
	rm -rf gbfile-read
	rm -rf gbfile-mmap-read
	rm -rf gbfile-mmap-write
