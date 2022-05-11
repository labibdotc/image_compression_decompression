//extern functions to use in getDCT.c
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "math.h" // for rounding!
#include "bitpack.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "VCS.h"
#include "DCT.h"


void  VCSArrtoFourPixArr(A2Methods_T methods, A2Methods_UArray2 VCSArr, A2Methods_UArray2 fourPixArr);

void fourPixArrtoVCSArr(A2Methods_T methods, A2Methods_UArray2 VCSArr, A2Methods_UArray2 fourPixArr);

void assignFourPix(struct VCS* VCS1,struct VCS* VCS2,struct VCS* VCS3,
        struct VCS* VCS4,struct fourPix* currfourPix, A2Methods_T methods);

void fourPixtoVCS(struct VCS* VCS1,struct VCS* VCS2,struct VCS* VCS3,
        struct VCS* VCS4,struct fourPix* currfourPix, A2Methods_T methods);

