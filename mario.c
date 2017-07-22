#include<stdio.h>
#include<cs50.h>
#include<stdlib.h>
int main(void)
{
    printf("Height: ");
    int h=get_int();
    while(h<=0 || h>23)
    {
        if(h==0)
        exit(0);
        printf("Height: ");
        h=get_int();
    }
    for(int i=0;i<h;i++)
    {
        for(int j=0;j<(h+1);j++)
        {
            if((i+j)>=(h-1))
            printf("#");
            else
            printf(" ");
        }
        printf("\n");
    }
}