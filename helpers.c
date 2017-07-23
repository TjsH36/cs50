/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int m,i,k;
    int narr[n];
    if(n<=0)
    return false;
    else
    {
        while(n>0)
        {
            m=n/2;
            if(values[m]==value)
            return true;
            else{
                if(values[m]>value)
                {
                    return search(value,values,m);
                }
                else
                {
                    for(i=m+1,k=0;i<n;i++,k++)
                    {
                        narr[k]=values[i];
                    }
                    return search(value,narr,k);
                }
            }
        }
        return false;
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    int count[65536]={0};
    int i,k;
    for(i=0;i<n;i++)
    {
        count[values[i]]++;
    }
    for(i=0,k=0;i<65536;i++)
    {
        if(count[i]>0)
        {
            values[k]=i;
            k++;
        }
    }
}
