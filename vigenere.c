#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

int main(int argc, string argv[])
{
    if(argc!=2)
    {
        printf("ERROR");
        return 1;
    }
    string key=argv[1];
    int i,n;
    n=strlen(key);
    int ik[n];
    for(i=0;key[i]!='\0';i++)
    {
        if(!isalpha(key[i]))
        {
            printf("ERROR");
            return 1;
        }
        if(key[i]>='a')
        {
            ik[i]=((int)key[i])-97;
        }
        else if(key[i]>='A')
        {
            ik[i]=((int)key[i])-65;
        }
    }
    printf("plaintext: ");
    string pt=get_string();
    printf("ciphertext: ");
    int c=0;
    for(i=0;pt[i]!='\0';i++)
    {
        if(isalpha(pt[i]))
            {
                if((pt[i]+ik[(c%n)])>'z')
                {
                    printf("%c",(((pt[i]+ik[(c%n)])%122)+96));
                }
                else if((pt[i]+ik[(c%n)])>'Z' && (pt[i]<='Z'))
                {
                    printf("%c",((pt[i]+ik[(c%n)])%90)+64);
                }
                else
                {
                    printf("%c",pt[i]+ik[(c%n)]);   
                }
                c++;
             }
            else
            {
                printf("%c",pt[i]);
            }
    }
    printf("\n");
}