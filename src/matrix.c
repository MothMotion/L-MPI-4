#include "config.h"
#include "matrix.h"
#include "random.h"

#include <stdint.h>
#include <stdlib.h>

#include <mpi.h>

uint8_t MatInit(arr_t** mat, const uint32_t size) {
  mat = malloc(size * sizeof(void*));
  if(!mat)
    return 0;

  for(uint32_t i=0; i<size; ++i) {
    mat[i] = malloc(size * sizeof(arr_t));
    if(!mat[i])
      return 0;
  }
  return 1;
}

void MatDeinit(arr_t** mat, const uint32_t size) {
  for(uint32_t i=0; i<size; ++i)
    free(mat[i]);
  free(mat);
}

void MatPerformOper(arr_t** mat1, arr_t** mat2, arr_t** out, const uint32_t size,
                 void op(arr_t*, arr_t*, arr_t*, const uint32_t)) {
  #ifdef SERIAL
  for(uint32_t i=0; i<size; ++i)
    op(mat1[i], mat2[i], out[i], size);

  #else
  int32_t rank, thr_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &thr_size);
  uint32_t width = size / thr_size + (size%thr_size) ? 1 : 0;

  for(uint32_t i = rank*width; i<size && i<((rank+1)*width); ++i)
    op(mat1[i], mat2[i], out[i], size);

  #endif
}

void RandMat(arr_t** mat, const uint32_t size, const arr_t min_v, const arr_t max_v) {
  for(uint32_t i=0; i<size; ++i)
    Randomize(mat[i], size, min_v, max_v);
}
