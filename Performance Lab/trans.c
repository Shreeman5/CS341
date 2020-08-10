/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 *
 * Name: Shreeman Gautam
 * User ID: gautams
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  int row, column;
  int transpose;
  int one;
  
  //for the 61 by 67 matrix
  if(M == 61 && N == 67)
  {
    for(row = 0; row < N; row += 16)//shift rows by 16
    {
      for(column = 0; column < M; column += 16)//shift columns by 16
      {
	for(transpose=0; (transpose < 16) && (column + transpose) < 61; transpose++)//ensure that columns don't run past 61
	{ 
	  for(one = 0; one < 16 && (row + one) < 67; one++)//ensure that rows don't run past 67
	  {  
	    B[column + transpose][row + one]= A[row + one][column + transpose];//work within 16 by 16 blocks and assign every value of matrix A
	                                                               //into it's transposed position
	  }
	}
      }
    }
  }

  int two, three, four, five, six, seven, eight;
  //for the 32 by 32 matrix
  if(M == 32 && N == 32)
  {
    for (row = 0; row < N; row += 8)//shift rows by 8
    {
      for (column = 0; column < M; column += 8)//shift columns by 8
      {
        for(transpose = 0; transpose < 8; transpose++)//work within the 8 by 8 and transpose by 8 by 8 blocks
        {
          //store the 8 values from A in 8 variables
          one = A[row][column + transpose];
          two = A[row + 1][column + transpose];
          three = A[row + 2][column + transpose];
          four = A[row + 3][column + transpose];
          five = A[row + 4][column + transpose];
          six = A[row + 5][column + transpose];
          seven = A[row + 6][column + transpose];
          eight = A[row + 7][column + transpose];

          //assign the 8 variables to the B matrix, but in transposed positions
          B[column + transpose][row]= one;
          B[column + transpose][row + 1] = two;
          B[column + transpose][row + 2] = three;
          B[column + transpose][row + 3] = four;
          B[column + transpose][row + 4] = five;
          B[column + transpose][row + 5] = six;
          B[column + transpose][row + 6] = seven;
          B[column + transpose][row + 7] = eight;
        }
      }
    }
  }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

