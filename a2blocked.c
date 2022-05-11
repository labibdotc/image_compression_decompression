/*
 * COMP40       HW3: Locality
 * 
 *              a2blocked.c
 *
 * Date:        02/17/2022
 * Authors:     Caleb Pekowsky(cpekow01) and Abraham Park(ypark12)
 * 
 * Purpose:     Implementation of a2blocked functions.
 */
#include <string.h>
#include <a2blocked.h>
#include "uarray2b.h"
#include <stdio.h>

// define a private version of each function in A2Methods_T that we implement

typedef A2Methods_UArray2 A2;	// private abbreviation

/*
 * Name:  new          
 * Input and Output: creates new A2, takes in width height and blocksize
 * Implementation:     calls  UArray2b_new_64K_block
 */
static A2 new(int width, int height, int size)
{
	return UArray2b_new_64K_block(width, height, size);
}


/*
 * Name:   new_with_blocksize             
 * Input and Output: ints, for the width, height, and blocksize of the new A2
 * Implementation:      calls Uarray2b_new
 */
static A2 new_with_blocksize(int width, int height, int size, int blocksize)
{
	return UArray2b_new(width, height, size, blocksize);
}


/*
 * Name:       a2free         
 * Input and Output:    pointer to UArray2b
 * Implementation:    calls   UArray2b_free
 */
static void a2free(A2 * array2p)
{
	UArray2b_free((UArray2b_T *) array2p);
}


/*
 * Name:        width        
 * Input and Output:    A2 array2
 * Implementation:  calls    UArray2b_width
 */
static int width(A2 array2)
{
	return UArray2b_width(array2);
}


/*
 * Name:     height           
 * Input and Output:   A2 array2 
 * Implementation:   calls  UArray2b_height 
 */


static int height(A2 array2)
{
	return UArray2b_height(array2);
}


/*
 * Name:      size          
 * Input and Output:  A2 array2  
 * Implementation:     calls UArray2b_size 
 */
static int size(A2 array2)
{
	return UArray2b_size(array2);
}


/*
 * Name:    blocksize            
 * Input and Output:  A2 array2  
 * Implementation:   calls    UArray2b_blocksize
 */
static int blocksize(A2 array2)
{
	return UArray2b_blocksize(array2);
}


/*
 * Name:           at     
 * Input and Output:  an array2, and indices ints i and j  
 * Implementation:     calls  UArray2b_at
 */
static A2Methods_Object *at(A2 array2, int i, int j)
{
	return UArray2b_at(array2, i, j);
}

typedef void applyfun(int i, int j, UArray2b_T array2b, void *elem, void *cl);


/* ------------------------------------------------------------------------ */
static void map_block_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
	UArray2b_map(array2, (applyfun *) apply, cl);
}


struct small_closure {
	A2Methods_smallapplyfun *apply;
	void *cl;
};

/*
 * Name:    apply_small            
 * Input and Output:   ints i and j for the current indice, array2, the elem 
 * and a closure  
 * Implementation: creates a struct small_closure, and passes everything 
 * into an apply function
 */
static void apply_small(int i, int j, UArray2b_T array2, void *elem, void *vcl)
{
	struct small_closure *cl = vcl;
	(void)i;
	(void)j;
	(void)array2;
	cl->apply(elem, cl->cl);
}

/*
 * Name:    small_map_block_major            
 * Input and Output: A2 array2, A2Methods_smallapplyfun apply 
 * and a closure  
 * Implementation: creates a struct small_closure, and passes everything 
 * into an apply function
 */
static void small_map_block_major(A2 a2, A2Methods_smallapplyfun apply,
				  void *cl)
{
	struct small_closure mycl = { apply, cl };
	UArray2b_map(a2, apply_small, &mycl);
}

static struct A2Methods_T uarray2_methods_blocked_struct = {
	new,
	new_with_blocksize,
	a2free,
	width,
	height,
	size,
	blocksize,
	at,
	NULL,			// map_row_major
	NULL,			// map_col_major
	map_block_major,
	map_block_major,	// map_default
	NULL,			// small_map_row_major
	NULL,			// small_map_col_major
	small_map_block_major,
	small_map_block_major,	// small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_blocked = &uarray2_methods_blocked_struct;
