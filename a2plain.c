/*
yaaaaaay
*     a2plain.c
*     by Abe Park and Caleb Pekowsky, Feb 18 2022
*     assignment 3 - locality
*
define a private version of each function in 
 A2Methods_T that we implement. 
*/ 

#include <string.h>
#include <a2plain.h>
#include "uarray2.h"

typedef A2Methods_UArray2 A2;   // private abbreviation
typedef void applyfun(int i, int j, UArray2_T array2b, void *elem, void *cl);

/*
 * Name:  new              
 * Input: takes in ints width, height, and size, representing the width,
 height, and size of each element in bytes.
 * Output: an A2Methods Uarray2, which under the hood can be blocked or plain.
 * Implementation: calls the Uarray2_new function.
 */
static A2Methods_UArray2 new(int width, int height, int size) {
    return UArray2_new(width, height, size);
}

/*
 * Name:  new_with_blocksize              
 * Input: takes in ints width, height, size, and blocksize representing width,
 height, and size of each element in bytes. blocksize won't do anything.
 * Output: an A2Methods Uarray2, which under the hood can be blocked or plain.
 * Implementation: calls the Uarray2_new function. blocksize is voided, 
 because this is a regular UArray2, so it can't do anything with blocksize.
 */
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size, 
                                          int blocksize)
{
  (void) blocksize;
  return UArray2_new(width, height, size);
}


/*
 * Name:  map_row_major              
 * Input: takes in an A2Methods Uarray2, A2methods_applyfun apply, and a 
 * void pointer to a closure.
 * Output: functin is void, so nothing.
 * Implementation: calls UArray2_map_row_major, and applies the apply function
 * to every element of the Uarray2 in row major order, while updating the 
 * closure.
 */
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
  UArray2_map_row_major(uarray2, (applyfun *)apply, cl);
}

/*
 * Name:  map_col_major              
 * Input: takes in an A2Methods Uarray2, A2methods_applyfun apply, and a 
 * void pointer to a closure.
 * Output: functin is void, so nothing.
 * Implementation: calls UArray2_map_col_major, and applies the apply function
 * to every element of the Uarray2 in col major order, while updating the 
 * closure.
 */
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
  UArray2_map_col_major(uarray2, (applyfun *)apply, cl);
}

/* struct to make small_apply work. */
struct small_closure {
  A2Methods_smallapplyfun *apply; 
  void                    *cl;
};

/*
 * Name:  apply_small              
 * Input: takes in an A2Methods Uarray2, A2methods_applyfun apply, and a 
 * void pointer to a closure.
 * Output: functin is void, so nothing.
 * Implementation: calls UArray2_map_col_major, and applies the apply function
 * to every element of the Uarray2 in col major order, while updating the 
 * closure. This function is basically just something to pass into 
 * the small map functions.
 */
static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
  struct small_closure *cl = vcl;
  (void)i;
  (void)j;
  (void)uarray2;
  cl->apply(elem, cl->cl);
}

/*
 * Name:  small_map_row_major              
 * Input: takes in an A2Methods Uarray2, A2methods_applyfun apply, and a 
 * void pointer to a closure.
 * Output: functin is void, so nothing.
 * Implementation: calls UArray2_map_row_major, and applies the apply function
 * to every element of the Uarray2 in row major order, while updating the 
 * closure.
 */
static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_row_major(a2, apply_small, &mycl);
}

/*
 * Name:  small_map_col_major              
 * Input: takes in an A2Methods Uarray2, A2methods_applyfun apply, and a 
 * void pointer to a closure.
 * Output: functin is void, so nothing.
 * Implementation: calls UArray2_map_col_major, and applies the apply function
 * to every element of the Uarray2 in col major order, while updating the 
 * closure.
 */
static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_col_major(a2, apply_small, &mycl);
}

/*
 * Name:  a2free              
 * Input: takes in an A2Methods Uarray2 pointer.
 * Output: functin is void, so nothing.
 * Implementation: calls UArray2_free, and frees the deferenced pointer.
 */
static void a2free(A2Methods_UArray2 * array2p)
{
        UArray2_free((UArray2_T *) array2p);
}

/*
 * Name:  width              
 * Input: takes in an A2Methods Uarray2.
 * Output: int, representing the width of the current A2methods UArray2.
 * Implementation: calls UArray2_width.
 */
static int width(A2Methods_UArray2 array2)
{
        return UArray2_width(array2);
}

/*
 * Name:  height              
 * Input: takes in an A2Methods Uarray2.
 * Output: int, representing the height of the current A2methods UArray2.
 * Implementation: calls UArray2_height.
 */
static int height(A2Methods_UArray2 array2)
{
        return UArray2_height(array2);
}

/*
 * Name:  size              
 * Input: takes in an A2Methods Uarray2.
 * Output: int, representing the size of the elements of the 
   current A2methods UArray2.
 * Implementation: calls UArray2_size.
 */
static int size(A2Methods_UArray2 array2)
{
        return UArray2_size(array2);
}
/*
 * Name:  blocksize              
 * Input: takes in an A2Methods Uarray2.
 * Output: int, which is always 1
 * Implementation: on a2m.h line 54, calling blocksize on unblocked array 
 * should return 1.
 */
static int blocksize(A2Methods_UArray2 array2)
{
    (void) array2;
        return 1;
}
/*
 * Name:  at              
 * Input: takes in an A2Methods Uarray2, and an int i and j, where i is 
 * column, and j is the row.
 * Output: A2Methods Object, representing whats at that index.
 * Implementation: calls UArray2_at.
 */
static A2Methods_Object *at(A2Methods_UArray2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}



/* This is the unblocked A2Methods struct. */
static struct A2Methods_T uarray2_methods_plain_struct = {
  new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,                  // map_row_major
        map_col_major,                  // map_col_major
        NULL,
        map_row_major,  // map_default
        small_map_row_major,                    // small_map_row_major
        small_map_col_major,                    // small_map_col_major
        NULL, 
        small_map_row_major,    // small_map_default

};

/* finally the payoff: here is the exported pointer to the struct */
A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
