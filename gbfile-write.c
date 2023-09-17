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
    FILE* fp;
	fp=fopen("./gb1.bin", "rb+");
    if(fp==NULL)
    {
        perror("Fopen");
        exit(-1);
    }

    for(int i=0;i<262144;i++)
    {   
        long address=i*4096;
        printf("Address is %lx",address);
        int fseekret=fseek(fp, address, SEEK_SET);
        if(fseekret==-1)
        {
            perror("fseek");
		    exit(-1);
        }

        int len_read= fread(buf, 1, 4096, fp);
        if(len_read>0)
        {
            if(len_read==4096)
            {   
                count++;
                *((int *) (&buf[32]))=i; //writing the index at the 32nd position
                for(int j=32;j<36;j++)
                {
                    if(j%8==0)
                    {
                        printf("\n");
                    }
                    printf("%0x ", buf[j]&0xff); //verifying we wrote it correctly

                }
                printf("\n");
                fseekret=fseek(fp, address, SEEK_SET);
                printf("Address is %lx",address);
                if(fseekret==-1)
                    {
                        perror("fseek");
                        exit(-1);
                    }
                int len_write= fwrite(buf,1, 4096, fp);
                if (len_write!=4096)
                {
                    perror("fwrite");
                    exit(-1);
                }
            }
        }
        else
        {
            if ( ferror(fp) ) 
            {
                perror( "Error reading file" );
                exit(-1);
            }
            else if ((feof(fp)))
            {   
                printf("Address %d \n", i);
                perror("EOF");
                exit(-1);
            }

        }

    }
    printf("Count of pages read %d \n", count);
	fclose(fp);
}
	