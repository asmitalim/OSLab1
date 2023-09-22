#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <ctype.h>
#include "gbfile.h"


char* globalptr=NULL;
int main()
{
	int fd=open("./gbdir/gb1.bin", O_RDWR);
	if (fd==-1){
		perror("open");
		exit(-1);
	}
	
	struct stat statbuf;
	int status; 
	status=fstat(fd, &statbuf);	
	if ((status<0)){
		perror("fstat");
		exit(-1);
	}
	size_t filesize;
	filesize=statbuf.st_size;	
    int prot;
    int flags;
    prot=PROT_READ | PROT_WRITE;
    flags=MAP_SHARED;

	char * memptr;
	memptr=mmap(0, filesize, prot, flags, fd, 0);
	if (memptr==MAP_FAILED){
		perror("mmap");
		exit(-1);
	}

    for(int i=0;i<LAB_BLOCK_COUNT;i++)
    {   
        long offset=i*LAB_BLOCK_SIZE;
        //printf("Offset is %lx",offset);
        char* blockptr= memptr+offset;
        int read_index;
        read_index= *((int *) (&blockptr[32]));
        if(read_index!=i)
        {
            printf("Index at 32nd location not read correctly. Read index: %d, index: %d \n",read_index,i);
        }
        char read_byte=blockptr[0];
        char modulo_idx=(char)(i&0xff);
        if(read_byte!= modulo_idx)
        {
            printf("Index at 0th location not read correctly. Read index: 0x %x, index: 0x%x \n",read_byte,modulo_idx);
        }
        /*else
        {
            printf("Index at 0th location read correctly . Read index: 0x%x, index: 0x%x \n",read_byte,modulo_idx);
        }
        */
    }
    munmap(memptr,filesize);
    close(fd);
}
