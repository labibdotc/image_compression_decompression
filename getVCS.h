#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "VCS.h"
//extern functions to use in getVCS.c

extern Pnm_ppm VCSArrToPnm(A2Methods_UArray2 VCSArr, A2Methods_T methods);
 
extern void VCSToRGBMap(int i, int j, A2Methods_UArray2 array2, 
                              A2Methods_Object *elem, void *cl);

extern void fillVCSArr(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl);

float normalizechroma(float chroma, unsigned denom);
