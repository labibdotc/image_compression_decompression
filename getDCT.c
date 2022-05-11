
/**************************************************************
*
*                     getDCT.c
*
*     Assignment: arith
*     Authors:  Caleb Pekowsky, Labib 
*     Date:     03/09/2022
*
*     summary:
*
*     this is the .c file for the DCT functions. The "secret" for these functions
*     is converting between the DCT struct, and a codeword.
*      
**************************************************************/

#include "getDCT.h"
#include "arith40.h"
#include "assert.h"
#include "except.h"
#include "VCS.h"
#include "DCT.h"


/*  Compression
*  Purpose: transfers picture from VCS to DCT (compression)
*  Parameters: A2Methods_T methods: methods 
*  A2Methods_UArray2 VCSArr: decompressed vcs array
*  A2Methods_UArray2 fourPixArr: array of DCT (quarter size of VCS array
*  Return: none
*  Effect: contracts four VCS pixels to one object represented by DCT values
*/
void  VCSArrtoFourPixArr(A2Methods_T methods, A2Methods_UArray2 VCSArr, A2Methods_UArray2 fourPixArr ) {    

    for(int i = 0; i < (methods -> height(VCSArr)); i+=2) {
        for(int j = 0; j < (methods -> width(VCSArr)); j+=2) {

            struct VCS *VCS3 = methods -> at(VCSArr, j, i);
            struct VCS *VCS4 = methods -> at(VCSArr, j+1, i);
            struct VCS *VCS1 = methods -> at(VCSArr, j, i+1);
            struct VCS *VCS2 = methods -> at(VCSArr, j+1, i+1);

            struct fourPix* currfourPix = methods -> at(fourPixArr, j/2, i/2);

            assignFourPix(VCS1, VCS2, VCS3, VCS4, currfourPix, methods);
        }
    }

}

/* Compression
*  Purpose: gets DCT of 4 pixels given four VCS 
*  Parameters: A2Methods_T methods: methods 
*  four struct VCS pointers: has the four VCS values
*  pointer to struct fourPix: to retrieve the DCT values of four pixels from the function
*  Effect: load DCT values of four pixels from four VCS objects  
*  Return: none
*/
void assignFourPix(struct VCS* VCS1,struct VCS* VCS2,struct VCS* VCS3,
        struct VCS* VCS4,struct fourPix* currfourPix, A2Methods_T methods) {

            float avgpb = ((VCS1 -> pb) + (VCS2 -> pb) +(VCS3 -> pb) +(VCS4 -> pb) )/(float)4.0;
            float avgpr = ((VCS1 -> pr) + (VCS2 -> pr) +(VCS3 -> pr) +(VCS4 -> pr) )/(float)4.0;

            float curra = ((VCS4 -> y) + (VCS3 -> y) + (VCS2 -> y) + (VCS1 -> y) )/(float)4.0;
            float currb = ((VCS4 -> y) + (VCS3 -> y) - (VCS2 -> y) - (VCS1 -> y) )/(float)4.0;
            float currc = ((VCS4 -> y) - (VCS3 -> y) + (VCS2 -> y) - (VCS1 -> y) )/(float)4.0;
            float currd = ((VCS4 -> y) - (VCS3 -> y) - (VCS2 -> y) + (VCS1 -> y) )/(float)4.0;

            currfourPix -> pb = avgpb;
            currfourPix -> pr = avgpr;

            currfourPix -> a = (curra);
            currfourPix -> b = (currb);
            currfourPix -> c = (currc);
            currfourPix -> d = (currd);

            (void)currfourPix;
            (void)methods;
}


/* Purpose: transfers picture from DCT to VCS *  Parameters: 
A2Methods_T methods: methods 
*  A2Methods_UArray2 DecompressedVCSArr: decompressed vcs array
*  A2Methods_UArray2 fourPixArr: array of DCT (quarter size of VCS array
*  Return: none
*  Effect: retrieves data of four pixels by one DCT object and load that onto the VCS array
*/
void fourPixArrtoVCSArr(A2Methods_T methods, A2Methods_UArray2 DecompressedVCSArr, A2Methods_UArray2 fourPixArr) {

    for(int i = 0; i < (methods -> height(DecompressedVCSArr)); i+=2) {
        for(int j = 0; j < (methods -> width(DecompressedVCSArr)); j+=2) {
            //printf("at %d %d \n",i,j);
            struct VCS *DCVCS3 = methods -> at(DecompressedVCSArr, j, i);
            struct VCS *DCVCS4 = methods -> at(DecompressedVCSArr, j+1, i);
            struct VCS *DCVCS1 = methods -> at(DecompressedVCSArr, j, i+1);
            struct VCS *DCVCS2 = methods -> at(DecompressedVCSArr, j+1, i+1);

            struct fourPix* currfourPix = methods -> at(fourPixArr, j/2, i/2);
 
            fourPixtoVCS(DCVCS1, DCVCS2, DCVCS3, DCVCS4, currfourPix, methods);

        }
    }
}





/* Purpose: transfers four pixels represented by a DCT to four VCS objects (decompression)
*  Parameters: A2Methods_T methods: methods 
*  four struct VCS pointers: to retrieve the new VCS objects
*  pointer to struct fourPix: that gives the DCT values of four pixels to the function
*  Effect: load four VCS objects with DCT values for four pixels
*/
void fourPixtoVCS(struct VCS* VCS1,struct VCS* VCS2,struct VCS* VCS3,
        struct VCS* VCS4,struct fourPix* currfourpix, A2Methods_T methods) {

        float y1 = (currfourpix -> a) - (currfourpix -> b) - (currfourpix -> c) + (currfourpix -> d);
        float y2 = (currfourpix -> a) -(currfourpix -> b) + (currfourpix -> c) - (currfourpix -> d);
        float y3 = (currfourpix -> a) + (currfourpix -> b) - (currfourpix -> c) - (currfourpix -> d);
        float y4 = (currfourpix -> a) + (currfourpix -> b) + (currfourpix -> c) + (currfourpix -> d);

        VCS1 -> pb = currfourpix -> pb;
        VCS1 -> pr = currfourpix -> pr;
        VCS1 -> y = y1;

        VCS2 -> pb = currfourpix -> pb;
        VCS2 -> pr = currfourpix -> pr;
        VCS2 -> y = y2;

        VCS3 -> pb = currfourpix -> pb;
        VCS3 -> pr = currfourpix -> pr;
        VCS3 -> y = y3;

        VCS4 -> pb = currfourpix -> pb;
        VCS4 -> pr = currfourpix -> pr;
        VCS4 -> y = y4;


        (void)methods;
}