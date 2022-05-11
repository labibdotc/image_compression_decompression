/*
 * COMP40       HW2: iii
 * 
 *              uarray.h
 *
 * Date:        02/07/2022
 * Authors:     Alex Jeon and Abraham Park
 * 
 * Purpose:     Interface of uarray2 function
 */



#ifndef _UARRAY2_H_
#define _UARRAY2_H_

#define T UArray2_T

typedef struct T *T;

T UArray2_new(int width, int height, int elm_size);
int UArray2_width(T arr);
int UArray2_height(T arr);
int UArray2_size(T arr);
void *UArray2_at(T arr, int col, int row);
void UArray2_map_col_major(T arr, 
     void apply(int x, int y, T arr_mapped, void *p1, void *p2), void *c1);
void UArray2_map_row_major(T arr, 
     void apply(int x, int y, T arr_mapped, void *p1, void *p2), void *c1);
void UArray2_free(T *arr);


#undef T
#endif