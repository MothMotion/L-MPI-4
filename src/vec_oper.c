#include "vec_oper.h"

#include <stdint.h>
#include <stddef.h>

#include <mpi.h>

void Summ(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size) {
  #ifdef SERIAL
  for(uint32_t i=0; i<size; ++i)
    arr_out[i] = arr1[i] + arr2[i];

  #else
  int32_t num_threads, cur_thread;
  MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_thread); 
  uint32_t width = size / num_threads + (size % num_threads) ? 1 : 0;

  for(uint32_t i = width * cur_thread; i < size && i < width * (cur_thread+1); ++i)
    arr_out[i] = arr1[i] + arr2[i];

  MPI_Barrier(MPI_COMM_WORLD);

  #endif
}

void Diff(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size) {
  #ifdef SERIAL
  for(uint32_t i=0; i<size; ++i)
    arr_out[i] = arr1[i] - arr2[i];

  #else
  int32_t num_threads, cur_thread;
  MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_thread);
  uint32_t width = size / num_threads + (size % num_threads) ? 1 : 0;

  for(uint32_t i = width * cur_thread; i < size && i < width * (cur_thread+1); ++i)
    arr_out[i] = arr1[i] - arr2[i];

  MPI_Barrier(MPI_COMM_WORLD);

  #endif
}

void Mult(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size) {
  #ifdef SERIAL
  for(uint32_t i=0; i<size; ++i)
    arr_out[i] = arr1[i] * arr2[i];

  #else
  int32_t num_threads, cur_thread;
  MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_thread);
  uint32_t width = size / num_threads + (size % num_threads) ? 1 : 0;

  for(uint32_t i = width * cur_thread; i < size && i < width * (cur_thread+1); ++i)
    arr_out[i] = arr1[i] * arr2[i];

  MPI_Barrier(MPI_COMM_WORLD);

  #endif
}

void Div(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size) {
  #ifdef SERIAL
  for(uint32_t i=0; i<size; ++i)
    arr_out[i] = arr1[i] + arr2[i];

  #else
  int32_t num_threads, cur_thread;
  MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
  MPI_Comm_rank(MPI_COMM_WORLD, &cur_thread);
  uint32_t width = size / num_threads + (size % num_threads) ? 1 : 0;

  for(uint32_t i = width * cur_thread; i < size && i < width * (cur_thread+1); ++i)
    if(arr2[i])
      arr_out[i] = arr1[i] / arr2[i];
    // If arr2[i] == 0 consider it equal 1
    else arr_out[i] = arr1[i];

  MPI_Barrier(MPI_COMM_WORLD);

  #endif
}
