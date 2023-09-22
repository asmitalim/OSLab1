#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <ctype.h>
#include <sys/resource.h>
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
        char index=(char) i&0xff; //changing index to 1 byte
        blockptr[0]= index;
        
        
    }
	int ret=msync(memptr, filesize, MS_SYNC);
    if (ret==-1)
    {
        perror("Msync!");
        exit(-1);
    }
    ret=munmap(memptr,filesize);
    if (ret==-1)
    {
        perror("Munmap!");
        exit(-1);
    }
    ret=close(fd);
    if (ret==-1)
    {
        perror("Closing file!");
        exit(-1);
    }

}
