#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "gbfile.h"

char buf [4096];
int main()
{   
    int count=0;
    FILE* fd;
	fd=fopen("./gb1.bin", "rb");
    if(fd==NULL)
    {
        perror("Fopen");
        exit(-1);
    }

    for(int i=0;i<262144;i++)
    {
        long address=i*4096;
        int fseekret=fseek(fd, address, SEEK_SET);
        if(fseekret==-1)
        {
            perror("fseek");
		    exit(-1);
        }

        int len_read= fread(buf, sizeof(char), 4096, fd);
        if(len_read>0)
        {
            count++;
            int read_index=0;
            if(len_read==4096)
            {   
                read_index= *((int *) (&buf[32]));
                for(int j=32;j<36;j++)
                {
                    if(j%8==0)
                    {
                        printf("\n");
                    }
                    printf("%0x ", buf[j]&0xff); //verifying we wrote it correctly

                }
                printf("\n");
                
                if(read_index!=i)
                {
                    printf("We didn't write file correctly! \n");
                    printf("Index is %d \n", i);
                    printf("Read index is %d \n", read_index);
                    exit(-1);
                }
                else 
                {
                    printf("File read correctly! \n");
                    printf("Index is %d \n", i);
                    printf("Read index is %d \n", read_index);

                }
                
               //printf("Char is %x \n", *((unsigned char*)check));

            }
        }
        else
        {
            if ( ferror(fd) ) 
            {
                perror( "Error reading file" );
                exit(-1);
            }
            else if ((feof(fd)))
            {   
                printf("Address %d \n", i);
                perror("EOF");
                exit(-1);
            }

        }

    }
    printf("Count of pages read %d \n", count);
	fclose(fd);
}
	