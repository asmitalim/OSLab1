all: gbfile-write gbfile-read gbfile-mmap-read gbfile-mmap-write gbfile-mmap-random-write inputoutput
	echo "Building files"

gb1: 
	dd if=/dev/zero of=gbdir/gb1.bin bs=64M count=16

gb1clean:
	rm -f gbdir/gb1.bin

gbfile-write: gbfile-write.o
	gcc -o gbfile-write gbfile-write.o

gbfile-read: gbfile-read.o
	gcc -o gbfile-read gbfile-read.o

gbfile-mmap-read : gbfile-mmap-read.o
	gcc -o gbfile-mmap-read gbfile-mmap-read.o

gbfile-mmap-write : gbfile-mmap-write.o
	gcc -o gbfile-mmap-write gbfile-mmap-write.o

gbfile-mmap-random-write : gbfile-mmap-random-write.o
	gcc -o gbfile-mmap-random-write gbfile-mmap-random-write.o

inputoutput: inputoutput.o
	gcc -o inputoutput inputoutput.o

.c.o:
	gcc -c -D_GNU_SOURCE $<

dockerhost: 
	docker build -t asmitaubuntu:22.04 .

run:
	./gbfile-write
	./gbfile-read


clean: 
	rm -f *.o 
	rm -rf gbfile-write
	rm -rf gbfile-read
	rm -rf gbfile-mmap-read
	rm -rf gbfile-mmap-write
	rm -rf gbfile-mmap-random-write
	rm -rf inputoutput

