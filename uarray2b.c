/*
 * COMP40       HW3: Locality
 * 
 *              uarray2b.c
 *
 * Date:        02/17/2022
 * Authors:     Caleb Pekowsky(cpekow01) and Abraham Park(ypark12)
 * 
 * Purpose:     Implementation of uarray2b functions.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <mem.h> 
#include <stdlib.h>
#include <uarray.h>
#include <uarray2.h>
#include <uarray2b.h>

#define T UArray2b_T

struct T {
    UArray2_T blocks;
    int width;
    int height;
    int size;
    int blocksize;
    int b_width;
    int b_height;
};

static int inrange(T array2b, int col, int row)
{
    return (col > -1 && row > -1 && 
            col < array2b -> width && row < array2b -> height);
}

/*
 * Name:                UArray2b_new
 * Parameters:          receive int width, height, and size of the input file, 
 *                      and user defined int blocksize.
 * Logic:               initialize variables in UArray2b_T and populate 
 *                      each block of the uarray2b with UArray with UArray_new.
 */
T UArray2b_new (int width, int height, int size, int blocksize)
{
    assert(width > -1 && height > -1 && blocksize > 0);

    T new_arr;
    NEW(new_arr);

    new_arr -> width     = width;
    new_arr -> height    = height;
    new_arr -> size      = size;
    new_arr -> blocksize = blocksize;
    /* padding consideration */
    new_arr -> b_width  = (width % blocksize == 0)  ? width  / blocksize
                                                    : width  / blocksize + 1;
    new_arr -> b_height = (height % blocksize == 0) ? height / blocksize
                                                    : height / blocksize + 1;

    new_arr -> blocks = UArray2_new(new_arr -> b_width, new_arr -> b_height, 
                                  sizeof(UArray_T));
    for (int i = 0; i < new_arr -> b_height; i++) {
        for (int j = 0; j < new_arr -> b_width; j++) {
            UArray_T *block = UArray2_at(new_arr -> blocks, j, i);
            *block = UArray_new(blocksize * blocksize, size);
        }
    }

    return new_arr;
}

/*
 * Name:                UArray2b_new_64K_block
 * Parameters:          receive int width, height and size of the input file.
 * Logic:               initialize variables in UArray2b_T and populate 
 *                      UArray in each block of the uarray2b.
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
    assert(width > -1 && height > -1);
    /* 65536 = 2^16 */
    UArray2b_T new_arr = UArray2b_new(width, height, size, 
                                     (int)sqrt(65536 / size));
    return new_arr;
}

/*
 * Name:                UArray2b_free
 * Parameters:          receive UArray2_T array2b
 * Logic:               iterate through array2b and free each (UArray) block.
 *                      then free uarray2b.
 */
void UArray2b_free(T *array2b)
{
    assert(array2b && *array2b);
    for (int i = 0; i < (*array2b)->b_height; i++) {
        for (int j = 0; j < (*array2b)->b_width; j++) {
            UArray_T *block = UArray2_at((*array2b)->blocks, j, i);
            UArray_free(block);
        }
    }
    UArray2_free(&((*array2b) -> blocks));
    FREE(*array2b);
}

/*
 * Name:                UArray2b_width
 * Parameters:          receive UArray2_T array2b
 * Logic:               return array2b's width
 */
int UArray2b_width(T array2b)
{
    assert(array2b);
    return array2b -> width;
}

/*
 * Name:                UArray2b_height
 * Parameters:          receive UArray2_T array2b
 * Logic:               return array2b's height
 */
int UArray2b_height(T array2b)
{
    assert(array2b);
    return array2b -> height;
}

/*
 * Name:                UArray2b_size
 * Parameters:          receive UArray2_T array2b
 * Logic:               return array2b's size
 */
int UArray2b_size(T array2b)
{
    assert(array2b);
    return array2b -> size;
}

/*
 * Name:                UArray2b_blocksize
 * Parameters:          receive UArray2_T array2b
 * Logic:               return array2b's blocksize
 */
int UArray2b_blocksize(T array2b)
{
    assert(array2b);
    return array2b -> blocksize;
}

/*
 * Name:                UArray2b_at
 * Parameters:          receive UArray2b_T array2b and (int column, row)
 * Logic:               return void* of the UArray block element in the array2b
 */
void *UArray2b_at(T array2b, int column, int row)
{
    /* check for (column, row) not in range */
    assert(array2b && inrange(array2b, column, row));
    int b_size = array2b->blocksize;

    UArray_T *block = UArray2_at(array2b->blocks, 
                                 column / b_size, row / b_size);
    
    /* index calcuation of the element in the block.
     * 1. row % b_size:         get the i_th row in the current block
     * 2. (1) * b_size:         (1) + all the indices skipped
     * 3. (2) + col % b_size:   (2) + get the j_th col in the current row
     */
    return UArray_at(*block, (((row % b_size) * b_size) + (column % b_size )));
}

/*
 * Name:                UArray2b_map
 * Paramenters:         receive UArray2b_T array2b, void apply function, and 
 *                      void *cl
 * Logic:               iterate through each element of each block in the 
 *                      given UArray2b. call apply function to indiviudal
 *                      element excepet the ones in the padding range.
 */
void UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                                        void *elem, void *cl),
                                        void *cl)
{
    assert(array2b);
    int b_size = UArray2b_blocksize(array2b);

    for (int i = 0; i < array2b->b_height; i++) {
        for (int j = 0; j < array2b->b_width; j++) {
            UArray_T *block = UArray2_at(array2b->blocks, j, i);
            for (int b_i = 0; b_i < UArray_length(*block); b_i++) {
                /* index calcuation of the current (i, j); necessary to
                 * check if it is out of the range (if it within the padding
                 * of the UArray2b).
                 */
                int row = (i * b_size) + (b_i / b_size);
                int col = (j * b_size) + (b_i % b_size);
                if (inrange(array2b, col, row)) {
                        apply(col, row, array2b, UArray_at(*block, b_i), cl);
                }
            }
        }
    }
}