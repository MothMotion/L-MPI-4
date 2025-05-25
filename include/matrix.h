#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "config.h"

#include <stdint.h>

#include <mpi.h>

#define RANKED_SIZE ( (rank == mpi_size-1) ? (mat_size - rank*width) : (width) )


// its so f horrendous
#define MPI_SYNC(mat, mat_size, rank, mpi_size, width) {\
  if(rank == 0)\
    for(uint32_t i=width; i<mat_size; ++i)\
      MPI_Recv(mat[i], mat_size, MPI_UNSIGNED_CHAR, i/width, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
  else\
    for(uint32_t i=0; i<RANKED_SIZE; ++i)\
      MPI_Send(mat[i], mat_size, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);\
}
// ^ lord forgive me

#define MPI_BSPLIT(mat, mat_size, rank, source, mpi_size, width) {\
  if(rank == source) {\
    for(uint32_t i=0; i<mat_size; ++i)\
      if(i/width != source)\
        MPI_Send(mat[i], mat_size, MPI_UNSIGNED_CHAR, i/width, 0, MPI_COMM_WORLD);\
  } else\
    for(uint32_t i=0; i<RANKED_SIZE; ++i)\
      MPI_Recv(mat[i], mat_size, MPI_UNSIGNED_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
}



uint8_t MatInitRect(arr_t** mat, const uint32_t size_x, const uint32_t size_y);
uint8_t MatInitSqr(arr_t** mat, const uint32_t size);
void MatDeinit(arr_t** mat, const uint32_t size);

void MatPerformOper(arr_t** mat1, arr_t** mat2, arr_t** out, const uint32_t size_x, const uint32_t size_y,
                 void op(arr_t*, arr_t*, arr_t*, const uint32_t));

void RandMat(arr_t** mat, const uint32_t size, const arr_t min_v, const arr_t max_v);

#endif // !MATRIX_H
