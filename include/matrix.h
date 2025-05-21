#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "config.h"

#include <stdint.h>

#include <mpi.h>


// its so f horrendous
#define MPI_SYNC(mat, mat_size, rank, mpi_size, width) {\
  for(uint32_t cur_r=0; cur_r<mpi_size; ++cur_r)\
    for(uint32_t i=cur_r*width; i<(cur_r+1)*width && i<mat_size; ++i)\
      if(cur_r == rank) {\
        for(uint32_t j=0; j<mpi_size; ++j)\
          if(j != rank)\
            MPI_Send(mat[i], mat_size, MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);\
      }\
      else\
        MPI_Recv(mat[i], mat_size, MPI_UNSIGNED_CHAR, cur_r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
}
// ^ lord forgive me

#define MPI_BROADCAST(mat, mat_size, rank, source, mpi_size) {\
  if(rank == source) {\
    for(uint32_t i=0; i<mat_size; ++i)\
      for(uint32_t j=0; j<mpi_size; ++j)\
        if(j != source)\
          MPI_Send(mat[i], mat_size, MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);\
  } else\
    for(uint32_t i=0; i<mat_size; ++i)\
      MPI_Recv(mat[i], mat_size, MPI_UNSIGNED_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
}



uint8_t MatInit(arr_t** mat, const uint32_t size);
void MatDeinit(arr_t** mat, const uint32_t size);

void MatPerformOper(arr_t** mat1, arr_t** mat2, arr_t** out, const uint32_t size,
                 void op(arr_t*, arr_t*, arr_t*, const uint32_t));

void RandMat(arr_t** mat, const uint32_t size, const arr_t min_v, const arr_t max_v);

#endif // !MATRIX_H
