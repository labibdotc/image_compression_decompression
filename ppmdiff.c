/**************************************************************
*
*                     ppmdiff.c
*
*     Assignment: arith
*     Authors:  Caleb Pekowsky, Labib 
*     Date:     03/09/2022
*
*     summary:
*
*     this is the .c file for ppmdiff. This takes in 2 ppm files, and compares 
*     how different they are.
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include <math.h>


struct Transformer {
    Pnm_ppm secondPnm;
    struct A2Methods_T* methods;
    float totalDiff;
    float firstPnmDenom;
};

void findDifference(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl);

int main(int argc, char *argv[]) 
{
        /* this is the file pointer reading from file, or stdin */
        FILE *firstPnmFP = fopen(argv[1], "r");
        FILE *secondPnmFP = fopen(argv[2], "r");
        (void)argc;
        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        /* set layout and dimension of the output ppm */
        Pnm_ppm firstPnm = Pnm_ppmread(firstPnmFP, methods);
        Pnm_ppm secondPnm = Pnm_ppmread(secondPnmFP, methods);

        float totalDiff = 0;
        

        int totalfirst = methods -> width(firstPnm -> pixels) * methods ->height(firstPnm -> pixels);
        int totalsecond = methods -> width(secondPnm -> pixels) * methods ->height(secondPnm -> pixels);

        struct Transformer currHelper;
        
        int smallerTotal = -1;
        (void)smallerTotal;
        printf("total first is %d total second is %d \n", totalfirst, totalsecond);
        printf("first denominater is %u second is %u ",firstPnm -> denominator, secondPnm -> denominator );
        printf("total first denom is %d , total second denom is %d", firstPnm -> denominator, secondPnm -> denominator);
        if (totalfirst >= totalsecond) {
            smallerTotal = totalsecond;
            currHelper.secondPnm = firstPnm;
            currHelper.methods = methods;
            currHelper.totalDiff = totalDiff;
            currHelper.firstPnmDenom = secondPnm -> denominator;
            map(secondPnm->pixels, findDifference, (void *)&currHelper);


        }
        
        else {
            smallerTotal = totalfirst;
            currHelper.secondPnm = secondPnm;
            currHelper.methods = methods;
            currHelper.totalDiff = totalDiff;
            currHelper.firstPnmDenom = firstPnm -> denominator;

            map(firstPnm->pixels, findDifference, (void *)&currHelper);
        }
        
        printf("total pixel difference is %lf \n", currHelper.totalDiff);
        float denom = smallerTotal*3;
        float E = sqrt ((currHelper.totalDiff) / (denom) );
        printf("total first is %d \n", totalfirst);
        printf("percent difference is %f \n", E );

        
        Pnm_ppmfree(&firstPnm);
        Pnm_ppmfree(&secondPnm);

        return EXIT_SUCCESS;
}


void findDifference(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl) {

   
                                 
    struct Transformer *currTransformer = cl;
    Pnm_ppm secondPnm = (Pnm_ppm) (currTransformer -> secondPnm );
    A2Methods_T methods = (A2Methods_T) (currTransformer -> methods );

    float firstPnmDenom= currTransformer -> firstPnmDenom;
    //printf("first Pnm denom is %f \n", firstPnmDenom );
    Pnm_rgb firstPnmPixel = elem;
    Pnm_rgb secondPnmPixel = methods -> at(secondPnm -> pixels, i, j);
    

    if(i == 0 && j == 0 ) {
        printf("first denom is %f second denom is %u \n", firstPnmDenom, secondPnm -> denominator );
    }

    float firstPnmPixelRed = (firstPnmPixel -> red) / firstPnmDenom ;
    float secondPnmPixelRed = (secondPnmPixel -> red) / (float)(secondPnm -> denominator );

    float firstPnmPixelGreen = (firstPnmPixel -> green)/ firstPnmDenom ;
    float secondPnmPixelGreen = (secondPnmPixel -> green) / (float)(secondPnm -> denominator );

    float firstPnmPixelBlue = (firstPnmPixel -> blue)/ firstPnmDenom ;
    float secondPnmPixelBlue = (secondPnmPixel -> blue) / (float)(secondPnm -> denominator );

    //printf("firstpnm red is %f second pnm red is %f \n", firstPnmPixelRed, secondPnmPixelRed);
    
   // printf("squared difference of first and second red is %f \n", (pow( secondPnmPixelRed-firstPnmPixelRed, 2)));
    
    (currTransformer -> totalDiff ) += (pow( (secondPnmPixelRed-firstPnmPixelRed), 2.0)) + (pow((secondPnmPixelGreen - firstPnmPixelGreen), 2)) + (pow((secondPnmPixelBlue - firstPnmPixelBlue), 2));
    (void)array2;
                             }