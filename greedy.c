#include<stdio.h>
#include<cs50.h>
#include<math.h>
int main(void)
{
    float ch;
    printf("O hai! How much change is owed?\n");
    ch=get_float();
    while(ch<=0.0)
    {
        printf("How much change is owed?\n");
        ch=get_float();
    }
    int i =round(ch*100);
    int count=0;
    count=count +(i/25);
    i=i%(25);
    count=count+(i/10);
    i=i%10;
    count=count+(i/5);
    i=i%5;
    count=count+i;
    printf("%d\n",count);
}