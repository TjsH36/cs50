#include<stdio.h>
#include<cs50.h>
#include<stdlib.h>

#include "bmp.h"
int main(int argc, string argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }
    string infile = argv[1];
    string outfile = argv[2];
    
    //opening file
    FILE *inp = fopen(infile,"r");
    if(inp == NULL)
    {
        fprintf(stderr, "Input file not open\n");
        return 2;
    }
    FILE *otp = fopen(outfile,"w");
    if(otp == NULL)
    {
        fprintf(stderr, "Output file not open\n");
        return 3;
    }
    
    
    BITMAPFILEHEADER bf;
    fread(&bf,sizeof(BITMAPFILEHEADER),1,inp);
    
    BITMAPINFOHEADER bi;
    fread(&bi,sizeof(BITMAPINFOHEADER),1,inp);
    
     if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inp);
        fclose(otp);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
     fwrite(&bf,sizeof(BITMAPFILEHEADER),1,otp);
     fwrite(&bi,sizeof(BITMAPINFOHEADER),1,otp);
     
     int padding= (4 -(bi.biWidth*sizeof(RGBTRIPLE))%4)%4;
     
     for(int i= 0,biheight=abs(bi.biHeight);i<biheight;i++)
     {
         for(int j=0;j<bi.biWidth;j++)
         {
             RGBTRIPLE triple;
             
             fread(&triple,sizeof(RGBTRIPLE),1,inp);
             
             if(triple.rgbtBlue == 0xff && triple.rgbtGreen == 0xff && triple.rgbtRed == 0xff)
            {
                triple.rgbtBlue=0x00;
                triple.rgbtRed=0xff;
                triple.rgbtGreen=0x00;
            }
            
            fwrite(&triple,sizeof(RGBTRIPLE),1,otp);
         }
         fseek(inp, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, otp);
        }
     }
     fclose(inp);
     fclose(otp);
     return 0;
}