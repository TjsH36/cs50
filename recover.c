#include<stdio.h>
#include<cs50.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        fprintf(stderr,"Not proper arguments!\n");
        return 1;
    }
    if(strcmp(argv[1],"card.raw")!=0)
    {
        fprintf(stderr,"Wrong argument\n");
        return 2;
    }
    
    uint8_t buffer[512];
    FILE *file=fopen(argv[1],"r");
    if(file == NULL)
    {
        fprintf(stderr,"File didnot open properly\n");
        return 3;
    }
    int count=-1;
    int ret;
    char filename[8];
    do
    {
        ret=fread(&buffer,1,512,file);
        if(ret<512)
        break;
        if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3] & 0xf0)==0xe0)
        {
            count++;
            sprintf(filename,"%03i.jpg",count);
        }
        if(count>-1)
        {
            FILE *img=fopen(filename,"a");
            if(img==NULL)
            {
                fprintf(stderr,"FIle didnot open properly\n");
                return 3;
            }
            fwrite(&buffer,1,512,img);
            fclose(img);
        }
    }while(ret==512);
    fclose(file);
    return 0;
}