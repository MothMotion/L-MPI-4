#include "config.h"
#include "vec_oper.h"
#include "timer.h"
#include "matrix.h"

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

int main(int argc, char** argv) {
  srand(time(NULL));
  const uint32_t mat_size = ARRAY_SIZE;
  const uint32_t cycles   = CYCLES;

  #ifdef SERIAL
  arr_t **mat1 = malloc(mat_size * sizeof(arr_t*)),
        **mat2 = malloc(mat_size * sizeof(arr_t*)),
        **out  = malloc(mat_size * sizeof(arr_t*));
  MatInit(mat1, mat_size);
  MatInit(mat2, mat_size);
  MatInit(out, mat_size);

  #else
  MPI_Init(&argc, &argv);
  int32_t rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  uint32_t width = mat_size / size + (mat_size % size) ? 1 : 0;
  uint32_t ranked_size = RANKED_SIZE;

  arr_t **mat1, **mat2, **out;
  if(rank == 0) {
    mat1 = malloc(mat_size * sizeof(arr_t*));
    out  = malloc(mat_size * sizeof(arr_t*));
    MatInit(mat1, mat_size);
    MatInit(out, mat_size);
  } else {
    mat1 = malloc(ranked_size);
    out = malloc(ranked_size);
    MatInit(mat1, ranked_size);
    MatInit(out, ranked_size);

    if(rank == 1) {
      mat2 = malloc(mat_size * sizeof(arr_t*));
      MatInit(mat2, mat_size);
    } else {
      mat2 = malloc(ranked_size);
      MatInit(mat2, ranked_size);
    }
  }

  #endif

  float sum_time = 0.0, dif_time = 0.0,
        mul_time = 0.0, div_time = 0.0; 
 


  #ifndef SERIAL
  if(rank == 0)
    printf("Settings:\n\tARRAY_SIZE:\t%d\n\tCYCLES:\t%d\n\tElement size:\t%lu\n\tTHREADS_AMOUNT:\t%d\n",
           mat_size, cycles, sizeof(arr_t), size);

  for(uint32_t i=0; i<cycles; ++i) {
    if(rank == 0)
      RandMat(mat1, mat_size, MIN_RAND, MAX_RAND);

    if(rank == 1)
      RandMat(mat2, mat_size, MIN_RAND, MAX_RAND);

    MPI_BROADCAST(mat1, mat_size, rank, 0, size, width);
    MPI_BROADCAST(mat2, mat_size, rank, 1, size, width);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_TIME(MatPerformOper, sum_time, cycles, rank, mat1, mat2, out, mat_size, Sum); 
    ADDTIME_COR(MPI_SYNC, sum_time, cycles, out, mat_size, rank, width);

    MPI_TIME(MatPerformOper, dif_time, cycles, rank, mat1, mat2, out, mat_size, Dif);
    ADDTIME_COR(MPI_SYNC, dif_time, cycles, out, mat_size, rank, width);

    MPI_TIME(MatPerformOper, mul_time, cycles, rank, mat1, mat2, out, mat_size, Mul);
    ADDTIME_COR(MPI_SYNC, mul_time, cycles, out, mat_size, rank, width);

    MPI_TIME(MatPerformOper, div_time, cycles, rank, mat1, mat2, out, mat_size, Div);
    ADDTIME_COR(MPI_SYNC, div_time, cycles, out, mat_size, rank, width);
  }
  MPI_Finalize();
  if(rank == 0)
    printf("Parallel execution:\n\tSum:\t%f\n\tDif:\t%f\n\tMul:\t%f\n\tDiv:\t%f\n",
         sum_time, dif_time, mul_time, div_time);

  #else
  printf("Settings:\n\tARRAY_SIZE:\t%d\n\tCYCLES:\t%d\n\tElement size:\t%lu\n",
         mat_size, cycles, sizeof(arr_t));

  for(uint32_t i=0; i<cycles; ++i) {
    RandMat(mat1, mat_size, MIN_RAND, MAX_RAND);
    RandMat(mat2, mat_size, MIN_RAND, MAX_RAND);

    ADDTIME_COR(MatPerformOper, sum_time, cycles, mat1, mat2, out, mat_size, Sum);
    ADDTIME_COR(MatPerformOper, dif_time, cycles, mat1, mat2, out, mat_size, Dif);
    ADDTIME_COR(MatPerformOper, mul_time, cycles, mat1, mat2, out, mat_size, Mul);
    ADDTIME_COR(MatPerformOper, div_time, cycles, mat1, mat2, out, mat_size, Div);
  }

  printf("Serial execution:\n\tSum:\t%f\n\tDif:\t%f\n\tMul:\t%f\n\tDiv:\t%f\n",
         sum_time, dif_time, mul_time, div_time);

  #endif

  MatDeinit(mat1, mat_size);
  MatDeinit(mat2, mat_size);
  MatDeinit(out, mat_size);

  return 0;
}
