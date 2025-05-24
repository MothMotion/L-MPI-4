#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "config.h"

#include <stdint.h>

#include <mpi.h>

#define RANKED_SIZE MIN(width, mat_size - rank*width)

// its so cute now :)
#define MPI_SYNC(mat, mat_size, rank, width) {\
  if(rank == 0)\
    for(uint32_t i=1; i<mat_size; ++i)\
      MPI_Recv(mat[i], mat_size, MPI_UNSIGNED_CHAR, i/width, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
  else\
    for(uint32_t i=0; i<RANKED_SIZE; ++i)\
      MPI_Send(mat[i], mat_size, MPI_UNSIGNED_CHAR, i/width, 0, MPI_COMM_WORLD);\
}
// ^ lord have forgiven me

#define MPI_BROADCAST(mat, mat_size, rank, source, mpi_size, width) {\
  if(rank == source) {\
    for(uint32_t i=0; i<mat_size; ++i)\
      if(i < source*width && i > (source+1)*width)\
        MPI_Send(mat[i], mat_size, MPI_UNSIGNED_CHAR, i/width, 0, MPI_COMM_WORLD);\
  } else\
    for(uint32_t i=0; i<RANKED_SIZE; ++i)\
      MPI_Recv(mat[i], mat_size, MPI_UNSIGNED_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
}



uint8_t MatInit(arr_t** mat, const uint32_t size);
void MatDeinit(arr_t** mat, const uint32_t size);

void MatPerformOper(arr_t** mat1, arr_t** mat2, arr_t** out, const uint32_t size,
                 void op(arr_t*, arr_t*, arr_t*, const uint32_t));

void RandMat(arr_t** mat, const uint32_t size, const arr_t min_v, const arr_t max_v);

#endif // !MATRIX_H
