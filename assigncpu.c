#include <sched.h>
#include<stdlib.h>
#include<stdio.h>
#ifdef TESTASSIGNCPU
int main()
{   
    int assigncpu();
    int x;
    x=assigncpu();
    printf("%d \n", x);

}
#endif
int assigncpu()
{
    size_t cpusetsize;
    cpu_set_t cpuset;
    int cpu;
    int ret;
    cpu=sched_getcpu();
    if(cpu==-1)
    {
        perror("Sched_getcpu!");
        exit(-1);
    }
    //printf("%d \n", cpu);
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    cpusetsize=sizeof(cpuset);
    ret=sched_setaffinity(0,cpusetsize,&cpuset);
    if(ret==-1)
    {
        perror("Sched_setaffinity!");
        exit(-1);
    }
    cpu=sched_getcpu();
    if(cpu==-1)
    {
        perror("Sched_getcpu!");
        exit(-1);
    }
    return cpu;

}