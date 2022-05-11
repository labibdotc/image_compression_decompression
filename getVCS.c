/**************************************************************
*
*                     getVCS.c
*
*     Assignment: arith
*     Authors:  Caleb Pekowsky, Labib 
*     Date:     03/09/2022
*
*     summary:
*
*     this is the .c file for getVCS. these functions convert ppm images to uarray2's holding VCS,
*     and vice versa.
**************************************************************/

#include "getVCS.h"


/* Purpose: converts a uarray2 holding VCS data, to a pnm_ppm image
*  Parameters: A2Methods_UArray2 of pixels' VCS and A2Methods
*  Returns: Pnm_ppm, holding the decompressed image. 
*/
extern Pnm_ppm VCSArrToPnm(A2Methods_UArray2 VCSArr, A2Methods_T methods) {
    Pnm_ppm outputPnm = malloc(sizeof(struct Pnm_ppm));
    
    //should this be 256? IDK, this is arbitrary. Maybe try making 
    //Denominator bigger, see if that makes images more accurate?
    outputPnm->denominator = 255;
    outputPnm->methods = methods;
    outputPnm->width = methods -> width(VCSArr);
    outputPnm->height = methods -> height(VCSArr);
    
    outputPnm -> pixels = methods->new(outputPnm -> width, 
        outputPnm -> height, sizeof( struct Pnm_rgb) );
        
    
    methods -> map_row_major(VCSArr, VCSToRGBMap, outputPnm);
    
    return outputPnm;

}
 

/* Purpose: mapping function to go through a UArray2
*  Parameters: 
    int i, j: coordinates of the current thing in the mapping
    A2Methods_UArray2 array2, the array being mapped
    A2Methods_Object *elem, the current elem while being mapped
    *cl, the closure, in this case it's the Pnm_ppm, which pixels we want to map
    into a UArray holding VCS data
*  Returns: void, so nothing. 
*/
extern void VCSToRGBMap(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl) 
{
    struct VCS *currVCS = elem;
    Pnm_ppm inputPnm = cl;
    Pnm_rgb currPixel = inputPnm -> methods -> at(inputPnm -> pixels, i, j);

    float pb = currVCS -> pb;
    float pr = currVCS -> pr;
    float y = currVCS -> y;

    float r = (1.0 *y) + (1.402*pr);
    float g = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr );
    float b = (1.0 * y) + (1.772 * pb);

    currPixel -> red = normalizechroma(r, inputPnm -> denominator);
    currPixel -> green = normalizechroma(g, inputPnm -> denominator);
    currPixel -> blue = normalizechroma(b, inputPnm -> denominator);
    
   (void)array2;
}

/* Purpose: make sure an rgb value is within the proper bounds, so 0 < value < denominator
*  Parameters: float chroma which is the r/g/b value, and unsigned denom, the denom of the pnm_ppm
*  Returns: float, which is gauranteed to be between 0 and denominator. 
*/
float normalizechroma(float chroma, unsigned denom) {

    chroma *=  (float)denom;

    if(chroma < 0) {
        return 0;
    }
    else if (chroma > denom) {
        return denom;
    }

    else {
        return chroma;
    }
}

/* Purpose: mapping function to fill the VCS Array with valid values from a ppm image
*  Parameters: 
    int i, j: coordinates of the current thing in the mapping
    A2Methods_UArray2 array2, the array being mapped, which is holding VCS structs
    A2Methods_Object *elem, the current elem while being mapped
    *cl, the closure, in this case it's the Pnm_ppm, which pixels we want to map
    into a UArray holding VCS data

*  Returns: float, which is gauranteed to be between 0 and denominator. 
*/
extern void fillVCSArr(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl) 
{
     Pnm_ppm inputPnm = cl;
     struct VCS *currVCS = elem;
     
     Pnm_rgb currPixel = inputPnm -> methods -> at(inputPnm -> pixels, i, j);
     
     float r = ((float)(currPixel -> red) / (float)(inputPnm -> denominator ));
     float g = ((float)(currPixel -> green) / (float)(inputPnm -> denominator ));
     float b = ((float)(currPixel -> blue) / (float)(inputPnm -> denominator ));


     currVCS -> y = ((float)0.299 * r) + ((float)0.587 * g) + ((float)0.114 * b);
     currVCS -> pb = ((float)-0.168736 * r) - ((float)0.331264 * g) + ((float)0.5 * b);
     currVCS -> pr = ((float)0.5 * r) - ((float)0.418688 * g) - ((float)0.081312 * b );

    (void)array2;
}

