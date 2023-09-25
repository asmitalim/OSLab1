#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/resource.h>
#include "gbfile.h"
#define	IO_SIZE 4096

char buf [LAB_BLOCK_SIZE];
void shuffle(uint64_t array[], int n)
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
void do_file_io(int fd, char *buf, 
      uint64_t *offset_array, size_t n, int opt_read)
{
  int ret = 0;
  for (int i = 0; i < n; i++) {
    ret = lseek(fd, offset_array[i], SEEK_SET);
    if (ret == -1) {
      perror("lseek");
      exit(-1);
    }
    if (opt_read)
      ret = read(fd, buf, IO_SIZE);
    else
      ret = write(fd, buf, IO_SIZE);
    if (ret == -1) {
      perror("read/write");
      exit(-1);
    }
  }
}
int main(int argc, char** argv)
{   
    int cpu;
    cpu=assigncpu();
    int who = RUSAGE_SELF;
    struct rusage ruse;
    int seq_flag=0;
    int opt_read=0;
    int headerinfo = 0;
    int c;
    int fd;
    size_t filesize;
    while ((c = getopt(argc, argv, "srh")) != -1)
    {
        switch (c)
        {
        case 's':
            seq_flag=1;
            break;
        case 'r':
            opt_read=1;
            break;
        case 'h':
            headerinfo = 1;
            break;

        default:
            printf("Uses: s for sequential, r for write, h to print header info. Default is random write");
            break;
        }
    }


    fd = open("./gbdir/gb1.bin", O_RDWR|O_DIRECT);
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
    

    uint64_t mapping[LAB_BLOCK_COUNT];
    for (int i = 0; i < LAB_BLOCK_COUNT; i++)
    {
        mapping[i] = i*LAB_BLOCK_SIZE; // recording the index
    }
    if(seq_flag==0)
    {
        shuffle(mapping, LAB_BLOCK_COUNT);
    }
    do_file_io(fd,buf,mapping,LAB_BLOCK_COUNT,opt_read);



    

    
    int ret = close(fd);
    if (ret == -1)
    {
        perror("Closing file!");
        exit(-1);
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
    char rw_buf[50];
    char seqran_buf[50];

    sprintf(rw_buf,"%s", opt_read? "Read":"Write");
    sprintf(seqran_buf, "%s", seq_flag?"Sequential":"Random");
    
    if(headerinfo==1)
    {
        printf("ReadOrWrite,SequentialOrRandom,TotalTime,Usertime,Systime,Maxrss,Minfaults,Majfaults,Inblock,Outblock,VolCSW,InvolCSV,Filesize,ProcessorCore\n");
    }
    printf("%s,%s,%6.3lf,%6.3lf,%6.3lf,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%d\n",rw_buf, seqran_buf, usec1+ssec1, usec1,ssec1,ruse.ru_maxrss, ruse.ru_minflt, ruse.ru_majflt, ruse.ru_inblock, ruse.ru_oublock, ruse.ru_nvcsw, ruse.ru_nivcsw, filesize,cpu);
}
