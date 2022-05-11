#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assert.h"


extern Pnm_ppm getEvenInputPnm( FILE* input, A2Methods_T methods);

extern void fixWidthAndHeight(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl);
