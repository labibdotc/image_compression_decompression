#include "IO.h"



/* Purpose: trims odd pnmppms
*  Parameters: pnmppm to get h
*  Return: Pnmppm trimmed
*
*/
extern Pnm_ppm getEvenInputPnm(FILE * input, A2Methods_T methods) {

    Pnm_ppm tempinputPNM = Pnm_ppmread(input, methods);

    Pnm_ppm newinputPNM = malloc(sizeof(struct Pnm_ppm));
    newinputPNM -> methods = tempinputPNM -> methods;
    newinputPNM -> denominator = tempinputPNM -> denominator;

    newinputPNM -> width = (tempinputPNM -> width);
    newinputPNM -> height = (tempinputPNM -> height);

    // if width and height are even
    /*
   if( (tempInputPNM -> height) % 2 == 0 && (tempInputPNM -> width) % 2 == 0) {
        newinputPNM -> width = (tempinputPNM -> width);
        newinputPNM -> height = (tempinputPNM -> height);
    }
    */
    
    if((tempinputPNM -> height) % 2 == 1) {
        newinputPNM -> height = (tempinputPNM -> height)-1;
        
    }
    
    if((tempinputPNM -> width) % 2 == 1) {
        newinputPNM -> width = (tempinputPNM -> width)-1;
    }
    //if width and height are odd 
    /*
    else if( (tempinputPNM -> height) % 2 == 1 && (tempinputPNM -> width) % 2 == 1) {
        printf("width and height are odd \n");
        newinputPNM -> width = (tempinputPNM -> width)-1;
        newinputPNM -> height = (tempinputPNM -> height)-1;

    }
    //if width is odd 
    else if( (tempinputPNM -> width) % 2 == 1 ) {
        printf("width is odd \n");
        newinputPNM -> width = (tempinputPNM -> width)-1;
        newinputPNM -> height = (tempinputPNM -> height);

    }
    //if height is odd 
    else {
        newinputPNM -> width = (tempinputPNM -> width);
        newinputPNM -> height = (tempinputPNM -> height)-1;
        //printf("height is odd \n");
    }
    */
    
    newinputPNM -> pixels = tempinputPNM -> methods->new(tempinputPNM -> width, 
        tempinputPNM -> height, sizeof( struct Pnm_rgb) );
    
    
    tempinputPNM -> methods->map_row_major(tempinputPNM -> pixels, 
        fixWidthAndHeight, newinputPNM);

    Pnm_ppmfree(&tempinputPNM);

    return newinputPNM;
    
}


/* Purpose: apply function to map, to simply copy everything to new pnm_ppm pixels array2.
*/
extern void fixWidthAndHeight(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl) 
{
    
        Pnm_ppm newPNM = cl;
        Pnm_rgb newPnmPixel = newPNM -> methods -> at(newPNM -> pixels, i, j);
        
        
        Pnm_rgb currOldPixel = elem;
        
        //printf("1 red at new is %d \n", newPnmPixel -> red);

        newPnmPixel -> red = currOldPixel -> red;
        newPnmPixel -> green = currOldPixel -> green;
        newPnmPixel -> blue = currOldPixel -> blue;

        //printf("2 red at new is %d \n", ((Pnm_rgb)(newPNM -> methods -> at(array2, i, j) )) -> red);
        (void)array2;
}
