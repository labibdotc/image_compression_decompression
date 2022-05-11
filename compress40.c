/**************************************************************
*
*                     compress40.c
*
*     Assignment: arith
*     Authors: Caleb Pekowsky, Labib 
*     Date:     03/09/2022
*
*     summary:
*
*     this is the .c file for compress40. This program takes in ppm images
*     and compresses them to sttdout, or takes in compressed images and decompresses them to stdout
*      
**************************************************************/
#include "compress40.h"
#include "IO.h"
#include "getVCS.h"
#include "getDCT.h"
#include "arith40.h"
#include "DCT.h"
#include "getword.h"

/*
Parameters: 

Purpose: 

Returns:
nothing.
*/


/* Purpose: Compress image through seven steps
*   Seven steps: 0- Read in commands
*                1- PNM -> RGB ints
*                2- RGB ints -> RGB ratios
*                3- RGB ratios -> VCS
*
* Return: none
* Effect: write out compressed image (1/3 initial size) to output stdout
*/
extern void compress40  (FILE *input) {

    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);
    
    //reading data into pnm 
    Pnm_ppm inputPnm;
    inputPnm = getEvenInputPnm(input, methods);
    
    //printf("valid width is %d height is %d \n", inputPnm -> width, inputPnm -> height);    
    // NOTE: need to check if it works on more pics

    //converting pnm into VCS Array
    A2Methods_UArray2 VCSArr = methods->new(inputPnm -> width, 
        inputPnm -> height, sizeof( struct VCS) );
        
    methods->map_row_major(VCSArr, fillVCSArr, inputPnm);

    // converting VCS array to DCT Array
    A2Methods_UArray2 fourPixArr = methods->new( (inputPnm -> width) / 2, 
        (inputPnm -> height) / 2, sizeof( struct fourPix) );

    VCSArrtoFourPixArr(methods, VCSArr, fourPixArr);
    
    //converting DCT to words (bitpacking included inside)
    //printf("COMP40 Compressed image format 2\n%u %u\n" , (methods->width(fourPixArr))*2, (methods->height(fourPixArr))*2); 
    //methods->map_row_major(fourPixArr, DCTtoWordMap, methods);
    printf("COMP40 Compressed image format 2\n%u %u\n" , (methods->width(fourPixArr))*2, (methods->height(fourPixArr))*2); 
    //methods -> map_row_major(fourPixArr, DCTtoWordMap, NULL);   
    //printf("COMP40 Compressed image format 2\n%u %u\n" , 180, 100); 
    methods -> map_row_major(fourPixArr, DCTtoWordMap, NULL);   

    //Decompression
    //Words to DCT


    //converting DCT array to VCS Array
    //A2Methods_UArray2 DecompressedVCSArr = methods->new( methods -> width(fourPixArr) * 2 , 
    //    methods -> height(fourPixArr) * 2, sizeof( struct VCS) );

    //fourPixArrtoVCSArr(methods, DecompressedVCSArr, fourPixArr);

    //step VCS -> PNM
    //Pnm_ppm outputPNM = VCSArrToPnm(DecompressedVCSArr, methods);
    //Pnm_ppmwrite(stdout, outputPNM);
    //DCTtoWords(VCSArr, methods);

    //Freeing heap
    Pnm_ppmfree(&inputPnm);
    //Pnm_ppmfree(&outputPNM);
    methods -> free(&fourPixArr);
    methods -> free(&VCSArr);
    //methods -> free(&DecompressedVCSArr);

        
}  /* reads PPM, writes compressed image */
extern void decompress40(FILE *input) {
    (void)input;
    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);

    A2Methods_UArray2 fourPixArr = compressedFiletoDCT(input, methods);

    A2Methods_UArray2 DecompressedVCSArr = methods->new( methods -> width(fourPixArr) * 2 , 
        methods -> height(fourPixArr) * 2, sizeof( struct VCS) );

    fourPixArrtoVCSArr(methods, DecompressedVCSArr, fourPixArr);

    Pnm_ppm outputPNM = VCSArrToPnm(DecompressedVCSArr, methods);
    (void)outputPNM;
    Pnm_ppmwrite(stdout, outputPNM);

    Pnm_ppmfree(&outputPNM);
    methods -> free(&fourPixArr);
    methods -> free(&DecompressedVCSArr);




}  /* reads compressed image, writes PPM */
