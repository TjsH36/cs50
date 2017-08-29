/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    
    
        float f=atof(argv[1]);
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
     //increasing widths and heights
     int com=bi.biHeight;
     int rom=bi.biWidth;
    bi.biWidth= bi.biWidth*f;
    bi.biHeight=bi.biHeight*f;
    
     // determine padding for scanlines
         // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    //change biSize
    bi.biSizeImage=((sizeof(RGBTRIPLE)*bi.biWidth)+padding)*abs(bi.biHeight);
    
    //Change bfSize
    bf.bfSize=bi.biSizeImage+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    int padold= (4 - (rom * sizeof(RGBTRIPLE)) % 4) % 4;

if(f>=1.0)
{
    // iterate over infile's scanlines
    for (int i = 0, biH = abs(com); i < biH; i++)
    {
        for(int x=0;x<f;x++){
        // iterate over pixels in scanline
        for (int j = 0; j < rom; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            for(int z=0;z<f;z++)
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
        fseek(inptr, -(long int)((sizeof(RGBTRIPLE))*rom),SEEK_CUR);}

        // skip over padding, if any
        fseek(inptr, padold+(sizeof(RGBTRIPLE)*rom), SEEK_CUR);

        // then add it back (to demonstrate how)
        
    }
}

else 
{
    int l=rom/bi.biWidth,somev;

    for (int i = 0, biH = abs(com); i < biH; i=i+l)
    {
            // iterate over pixels in scanline
            for (int j = 0; j < rom; j=j+l)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                fseek(inptr,(l-1)*(sizeof(RGBTRIPLE)),SEEK_CUR);
            }
            for (int k = 0; k < padding; k++)
            {

                fputc(0x00, outptr);
            }

        // skip over padding, if any
        fseek(inptr, padold, SEEK_CUR);
        somev=(long int)(l-1)*((sizeof(RGBTRIPLE)*rom)+padold);
        fseek(inptr,somev , SEEK_CUR);

        // then add it back (to demonstrate how)
        
    }
}
   
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
