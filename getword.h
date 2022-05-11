//extern functions to use in getDCT.c
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "math.h" // for rounding!
#include "bitpack.h"
#include "arith40.h"
#include "DCT.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assert.h"

int quantize(float value);

void writeAword(int32_t word);

void DCTtoWordMap(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl);

void compressedFiletoDCTMap(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl);
//float normalizechroma(float chroma);

A2Methods_UArray2 compressedFiletoDCT(FILE* in, A2Methods_T methods);