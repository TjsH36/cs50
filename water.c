#include<stdio.h>
#include<cs50.h>
int main(void)
{
    int min,bot;
    printf("Minutes: ");
    min=get_int();
    bot=12*min;
    printf("Bottles: %d\n",bot);
}