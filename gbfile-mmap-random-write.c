#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/resource.h>
#include "gbfile.h"

void shuffle(int array[], int n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
int main(int argc, char** argv)
{   
    int cpu;
    cpu=assigncpu();
    int who = RUSAGE_SELF;
    struct rusage ruse;
    int priv = 0;
    int anon = 0;
    int headerinfo = 0;
    int c;
    while ((c = getopt(argc, argv, "psah")) != -1)
    {
        switch (c)
        {
        case 'p':
            priv = 1;
            break;
        case 's':
            priv = 0;
            break;
        case 'a':
            anon = 1;
            break;
        case 'h':
            headerinfo = 1;
            break;

        default:
            printf("Uses: p for private, s for shared, a for anonymous, h to print header info");
            break;
        }
    }
    int prot;
    int flags = 0;
    prot = PROT_READ | PROT_WRITE;
    if (priv == 1)
    {
        flags = MAP_PRIVATE;
    }
    else
    {
        flags = MAP_SHARED;
    }
    if (anon == 1)
    {
        flags = flags | MAP_ANON;
    }
    int fd;
    size_t filesize;

    if (anon == 0)
    {
        fd = open("./gbdir/gb1.bin", O_RDWR);
        if (fd == -1)
        {
            perror("open");
            exit(-1);
        }

        struct stat statbuf;
        int status;
        status = fstat(fd, &statbuf);
        if ((status < 0))
        {
            perror("fstat");
            exit(-1);
        }
        filesize = statbuf.st_size;
    }
    else
    {
        fd = -1;
        filesize = (1 << 30);
    }

    char *memptr;
    memptr = mmap(0, filesize, prot, flags, fd, 0);
    if (memptr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }

    int mapping[LAB_BLOCK_COUNT];
    for (int i = 0; i < LAB_BLOCK_COUNT; i++)
    {
        mapping[i] = i; // recording the index
    }
    shuffle(mapping, LAB_BLOCK_COUNT);

    for (int i = 0; i < LAB_BLOCK_COUNT; i++)
    {
        /*long offset=i*LAB_BLOCK_SIZE;
        //printf("Offset is %lx",offset);
        char* blockptr= memptr+offset;
        char index=(char) i&0xff; //changing index to 1 byte
        blockptr[0]= index;*/
        int index;
        index = mapping[i];
        long offset = index * LAB_BLOCK_SIZE;
        char *blockptr = memptr + offset;
        blockptr[0] = (char)(index & 0xff);
    }
    int ret = msync(memptr, filesize, MS_SYNC);
    if (ret == -1)
    {
        perror("Msync!");
        exit(-1);
    }
    ret = munmap(memptr, filesize);
    if (ret == -1)
    {
        perror("Munmap!");
        exit(-1);
    }
    if (anon == 0)
    {
        ret = close(fd);
        if (ret == -1)
        {
            perror("Closing file!");
            exit(-1);
        }
    }
    ret = getrusage(who, &ruse);
    if (ret == -1)
    {
        perror("Rusage!");
        exit(-1);
    }
    double ssec1, usec1;
    ssec1 = (double)(ruse.ru_stime.tv_sec * 1000000 + ruse.ru_stime.tv_usec);
    ssec1= ssec1/1000000;
    usec1 = (double)(ruse.ru_utime.tv_sec * 1000000 + ruse.ru_utime.tv_usec);
    usec1=usec1/1000000;
    char map_buf[50];
    char file_buf[50];

    sprintf(map_buf,"%s", priv? "Private":"Shared");
    sprintf(file_buf, "%s", anon?"Anonymous":"FileBacked");
    
    if(headerinfo==1)
    {
        printf("PrivateOrNot,AnonymousOrNot,TotalTime,Usertime,Systime,Maxrss,Minfaults,Majfaults,Inblock,Outblock,VolCSW,InvolCSV,Filesize,ProcessorCore\n");
    }
    printf("%s,%s,%6.3lf,%6.3lf,%6.3lf,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%d\n",map_buf, file_buf, usec1+ssec1, usec1,ssec1,ruse.ru_maxrss, ruse.ru_minflt, ruse.ru_majflt, ruse.ru_inblock, ruse.ru_oublock, ruse.ru_nvcsw, ruse.ru_nivcsw, filesize, cpu);
}
