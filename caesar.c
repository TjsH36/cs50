#include<stdio.h>
#include<cs50.h>
#include<stdlib.h>
#include<ctype.h>

int main(int argc, string argv[])
{
    if(argc!=2)
    {
        printf("No comand line input \n");
        return 1;
    }
    int k;
    k=atoi(argv[1]);
    k=k%26;
        printf("plaintext: ");
        string pt=get_string();
        printf("ciphertext: ");
        for(int i=0;pt[i]!='\0';i++)
        {
            if(isalpha(pt[i]))
            {
            if((pt[i]+k)>'z')
            {
                printf("%c",(((pt[i]+k)%122)+96));
            }
            else if((pt[i]+k)>'Z' && (pt[i]<='Z'))
            {
                printf("%c",((pt[i]+k)%90)+64);
            }
            else{
             printf("%c",pt[i]+k);   
            }}
            else
            {
                printf("%c",pt[i]);
            }
        }
        printf("\n");
}
