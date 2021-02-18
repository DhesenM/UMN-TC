// optimized versions of matrix diagonal summing
#include "matvec.h"

int matsquare_VER1(matrix_t mat, matrix_t matsq) {
  int a, b;  // initialize a and b in the loop before unrolling
  for(int i = 0; i < mat.rows; i++)
  {
    int l = i * mat.cols;        // l = i*mat.cols (replace the second element in MGET and MSET)
    // loop unrolling 
    // set the value of first element in every column for one row
    for(a = 0; a < mat.cols - 2; a += 3) {      
      matsq.data[l + a] = mat.data[a] * mat.data[l];                   // MSET(mstsq, i, a, MGET(mat, 0, a) * MGET(mat, i, 0))
      matsq.data[l + a + 1] = mat.data[a + 1] * mat.data[l];           // MSET(mstsq, i, a + 1, MGET(mat, 0, a + 1) * MGET(mat, i, 0))
      matsq.data[l + a + 2] = mat.data[a + 2] * mat.data[l];           // MSET(mstsq, i, a + 2, MGET(mat, 0, a + 2) * MGET(mat, i, 0))
    }
    for(; a < mat.cols; a++) {   // finish any remaining columns 
      matsq.data[l + a] = mat.data[a] * mat.data[l];                   // MSET(mstsq, i, a, MGET(mat, 0, a) * MGET(mat, i, 0))
    }

    for(int j = 1; j < mat.cols; j++)
    { 
      int m = j * mat.cols;      // m = j*mat.cols (replace the second element in MGET and MSET)
      // loop unrolling 
      // set the value of the rest element in every column for one row
    for(b = 0; b < mat.cols - 2; b += 3) {
      matsq.data[l + b] += mat.data[l + j] * mat.data[m + b];           // MSET(mstsq, i, b, MGET(mat, i, j) * MGET(mat, j, b))
      matsq.data[l + b + 1] += mat.data[l + j] * mat.data[m + b + 1];   // MSET(mstsq, i, b + 1, MGET(mat, i, j) * MGET(mat, j, b + 1))
      matsq.data[l + b + 2] += mat.data[l + j] * mat.data[m + b + 2];   // MSET(mstsq, i, b + 2, MGET(mat, i, j) * MGET(mat, j, b + 2))
    }
    for(; b < mat.cols; b++) {   // finish any remaining columns 
      matsq.data[l + b] += mat.data[l + j] * mat.data[m + b];           // MSET(mstsq, i, b, MGET(mat, i, j) * MGET(mat, j, b))
    }
    }
  }
  return 0;  // return
}


int matsquare_OPTM(matrix_t mat, matrix_t matsq) {
  if(mat.rows != mat.cols ||                       // must be a square matrix to square it
     mat.rows != matsq.rows || mat.cols != matsq.cols)
  {
    printf("matsquare_OPTM: dimension mismatch\n");
    return 1;
  }


  // Call to some version of optimized code
  return matsquare_VER1(mat, matsq);
}
