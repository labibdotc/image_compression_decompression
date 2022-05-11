/**************************************************************
*
*                     DCT.h
*
*     Assignment: arith
*     Authors:  Caleb Pekowsky, Labib Afia
*     Date:     03/09/2022
*
*     summary:
*
*     this is the .h file holding the DCT struct. This represents
*     an intermediary step of the compression/decompression process.
*     
*      
**************************************************************/

#ifndef FOURPIX_H
#define FOURPIX_H

struct fourPix {
     float pb;
     float pr;
     float a;
     float b;
     float c;
     float d;
};

#endif