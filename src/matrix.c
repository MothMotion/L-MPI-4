#include "config.h"
#include "matrix.h"
#include "random.h"

#include <stdint.h>
#include <stdlib.h>

#include <mpi.h>

uint8_t MatInitRect(arr_t** mat, const uint32_t size_x, const uint32_t size_y) { 
  for(uint32_t i=0; i<size_x; ++i) {
    mat[i] = malloc(size_y * sizeof(arr_t));
    if(!mat[i])
      return 0;
  }
  return 1;
}

uint8_t MatInitSqr(uint8_t** mat, const uint32_t size) {
  return MatInitRect(mat, size, size);
}

void MatDeinit(arr_t** mat, const uint32_t size) {
  for(uint32_t i=0; i<size; ++i) {
    free(mat[i]);
    mat[i] = NULL;
  }
  free(mat);
}

void MatPerformOper(arr_t** mat1, arr_t** mat2, arr_t** out, const uint32_t size_x, const uint32_t size_y,
                 void op(arr_t*, arr_t*, arr_t*, const uint32_t)) {
  for(uint32_t i=0; i<size_x; ++i)
    op(mat1[i], mat2[i], out[i], size_y);
}

void RandMat(arr_t** mat, const uint32_t size, const arr_t min_v, const arr_t max_v) {
  for(uint32_t i=0; i<size; ++i)
    Randomize(mat[i], size, min_v, max_v);
}
