#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

int main(void)
{
    int i;
    string s=get_string();
    printf("%c",toupper(s[0]));
    for(i=0;s[i]!='\0';i++)
    {
        if(s[i]==' ')
        printf("%c",toupper(s[i+1]));
    }
    printf("\n");
} 