#pragma once
#ifndef TIMER
#define TIMER

#include <time.h>

#include <mpi.h>

#define GETTIME(func, time_var, ...) {\
  struct timespec start, end;\
  clock_gettime(CLOCK_REALTIME, &start);\
  func(__VA_ARGS__);\
  clock_gettime(CLOCK_REALTIME, &end);\
  time_var = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1e9;\
}

#define ADDTIME_COR(func, time_var, cor_val, ...) {\
  struct timespec start, end;\
  clock_gettime(CLOCK_REALTIME, &start);\
  func(__VA_ARGS__);\
  clock_gettime(CLOCK_REALTIME, &end);\
  time_var += ((double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1e9)/cor_val;\
}

#define MPI_TIME(func, time_var, cor_val, rank, ...) {\
  struct timespec start, end;\
  if(rank == 0)\
    clock_gettime(CLOCK_REALTIME, &start);\
  func(__VA_ARGS__);\ 
  if(rank == 0) {\
    clock_gettime(CLOCK_REALTIME, &end);\
    time_var += ((double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1e9)/cor_val;}\ 
}

#endif
