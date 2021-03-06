/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "NepalGuys",              /* Team name */

    "Shreeman Gautam",     /* First member full name */
    "gautams@unm.edu",  /* First member email address */

    "Amun Kharel",                   /* Second member full name (leave blank if none) */
    "akharel@unm.edu"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst)
{
    int i, j;
    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
  int row, column, rowSection, columnSection;
  for (row = 0; row < dim; row = row + 16)
  {
    for (column = 0; column < dim; column = column + 16)
    {
      for (rowSection = row; rowSection < row + 16; rowSection++)
      {
	for (columnSection = column; columnSection < column + 16; columnSection++)
        {
	  dst[RIDX(dim - 1 - columnSection, rowSection, dim)] = src[RIDX(rowSection, columnSection, dim)];
        }
      }
    }
  }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
}
