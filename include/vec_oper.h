#pragma once
#ifndef VEC_OPER_H
#define VEC_OPER_H

#include "config.h"
#include "random.h"

#include <stdint.h>
#include <stddef.h>

#include <mpi.h>

#define MPI_SYNC(arr, rank, mpi_size, arr_size, width) {\
  for(uint32_t i=0; i<mpi_size; ++i)\
    if(i == rank) {\
      for(uint32_t j=0; j<mpi_size; ++j)\
        if(j != rank)\
          MPI_Send(&arr[rank*width], MIN(width, arr_size - i*width), MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);\
    } else\
      MPI_Recv(&arr[i*width], MIN(width, arr_size - i*width), MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
}
#define MPI_BROADCAST(arr, count, rank, source, mpi_size) {\
  if(rank == source) {\
    for(uint32_t j=0; j<mpi_size; ++j)\
      if(j != source)\
        MPI_Send(arr, count, MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);\
  } else\
    MPI_Recv(arr, count, MPI_UNSIGNED_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
}

void Summ(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);
void Diff(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);
void Mult(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);
void Div(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);

#endif // !VEC_OPER_H
