/**
 * Implements a dictionary's functionality.
 */
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<string.h>
#include<strings.h>
#include "dictionary.h"
#include<ctype.h>

#define TABSIZE 100
struct node
{
    char word[LENGTH+1];
    struct node *next;
}*hashtab[TABSIZE];

unsigned int nwords=0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    unsigned int hashv=0;
    int val;
    for(int i =0;word[i]!='\0';i++)
        {
            hashv=(hashv*tolower(word[i])+tolower(word[i])+i)%TABSIZE;
        }
    struct node *cursor=hashtab[hashv];
    
    while(cursor!=NULL)
    {
        val=strcasecmp(cursor->word,word);
        if(val==0)
        return true;
        else
        cursor=cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    
    FILE *dict = fopen(dictionary,"r");
    
    if(dict==NULL)
    {
        printf("Dictionary could not be opened\n");
        return false;
    }
    char str[LENGTH+1];
    while(fscanf(dict,"%s",str)!= EOF)
    {
        struct node *newnode = malloc(sizeof(struct node));
        strcpy(newnode->word,str);
        
        unsigned int hashk=0;
        for(int i =0;str[i]!='\0';i++)
        {
            hashk=(hashk*str[i]+str[i]+i)%TABSIZE;
        }
        if(hashtab[hashk]==NULL)
        {
            newnode->next=NULL;
            hashtab[hashk]=newnode;
            
        }
        else
        {
            newnode->next=hashtab[hashk];
            hashtab[hashk]=newnode;
        }
        nwords++;
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    return nwords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int i=0;i<TABSIZE;i++)
    {
        struct node *cursor=hashtab[i];
        while(cursor!=NULL)
        {
            struct node *temp=cursor;
            cursor=cursor->next;
            free(temp);
        }
        free(cursor);
    }
    
    return true;
}
