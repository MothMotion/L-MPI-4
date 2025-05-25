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

  arr_t **mat1, **mat2, **out;

  float sum_time = 0.0, dif_time = 0.0,
        mul_time = 0.0, div_time = 0.0; 
 


  #ifndef SERIAL
  MPI_Init(&argc, &argv);
  int32_t rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  uint32_t width = mat_size / mpi_size + ((mat_size % mpi_size) ? 1 : 0);
  uint32_t ranked_size = RANKED_SIZE;
  
  if(rank == 0) {
    printf("Settings:\n\tARRAY_SIZE:\t%d\n\tCYCLES:\t%d\n\tElement size:\t%lu\n\tTHREADS_AMOUNT:\t%d\n",
           mat_size, cycles, sizeof(arr_t), mpi_size);

    mat1 = malloc(mat_size * sizeof(arr_t*));
    mat2 = malloc(ranked_size * sizeof(arr_t*));
    out = malloc(mat_size * sizeof(arr_t*));

    MatInitSqr(mat1, mat_size);
    MatInitRect(mat2, ranked_size, mat_size);
    MatInitSqr(out, mat_size);
  } else {
    mat1 = malloc(ranked_size * sizeof(arr_t*));
    out = malloc(ranked_size * sizeof(arr_t*));

    MatInitRect(mat1, ranked_size, mat_size);
    MatInitRect(out, ranked_size, mat_size);

    if(rank == 1) {
      mat2 = malloc(mat_size * sizeof(arr_t*));
      MatInitSqr(mat2, mat_size);
    } else {
      mat2 = malloc(ranked_size * sizeof(arr_t*));
      MatInitRect(mat2, ranked_size, mat_size);
    }
  }

  for(uint32_t i=0; i<cycles; ++i) {
    if(rank == 0)
      RandMat(mat1, mat_size, MIN_RAND, MAX_RAND);

    if(rank == 1)
      RandMat(mat2, mat_size, MIN_RAND, MAX_RAND);

    MPI_BSPLIT(mat1, mat_size, rank, 0, mpi_size, width);
    MPI_BSPLIT(mat2, mat_size, rank, 1, mpi_size, width);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_TIME(MatPerformOper, sum_time, cycles, rank, mat1, mat2, out, ranked_size, mat_size, Sum);
    ADDTIME_COR(MPI_SYNC, sum_time, cycles, out, mat_size, rank, mpi_size, width);

    MPI_TIME(MatPerformOper, dif_time, cycles, rank, mat1, mat2, out, ranked_size, mat_size, Dif);
    ADDTIME_COR(MPI_SYNC, dif_time, cycles, out, mat_size, rank, mpi_size, width);

    MPI_TIME(MatPerformOper, mul_time, cycles, rank, mat1, mat2, out, ranked_size, mat_size, Mul);
    ADDTIME_COR(MPI_SYNC, mul_time, cycles, out, mat_size, rank, mpi_size, width);

    MPI_TIME(MatPerformOper, div_time, cycles, rank, mat1, mat2, out, ranked_size, mat_size, Div);
    ADDTIME_COR(MPI_SYNC, div_time, cycles, out, mat_size, rank, mpi_size, width);
  }
  if(rank == 0) {
    printf("Parallel execution:\n\tSum:\t%f\n\tDif:\t%f\n\tMul:\t%f\n\tDiv:\t%f\n",
         sum_time, dif_time, mul_time, div_time);
    MatDeinit(mat1, mat_size);
    MatDeinit(mat2, ranked_size);
    MatDeinit(out, mat_size);
  } else {
    MatDeinit(mat1, ranked_size);
    MatDeinit(out, ranked_size);

    if(rank == 1)
      MatDeinit(mat2, mat_size);
    else
      MatDeinit(mat2, ranked_size);
  }

  MPI_Finalize();

  #else
  mat1 = malloc(mat_size * sizeof(arr_t*)),
  mat2 = malloc(mat_size * sizeof(arr_t*)),
  out  = malloc(mat_size * sizeof(arr_t*));
  
  MatInitSqr(mat1, mat_size);
  MatInitSqr(mat2, mat_size);
  MatInitSqr(out, mat_size);

  printf("Settings:\n\tARRAY_SIZE:\t%d\n\tCYCLES:\t%d\n\tElement size:\t%lu\n",
         mat_size, cycles, sizeof(arr_t));

  for(uint32_t i=0; i<cycles; ++i) {
    RandMat(mat1, mat_size, MIN_RAND, MAX_RAND);
    RandMat(mat2, mat_size, MIN_RAND, MAX_RAND);

    ADDTIME_COR(MatPerformOper, sum_time, cycles, mat1, mat2, out, mat_size, mat_size, Sum);
    ADDTIME_COR(MatPerformOper, dif_time, cycles, mat1, mat2, out, mat_size, mat_size, Dif);
    ADDTIME_COR(MatPerformOper, mul_time, cycles, mat1, mat2, out, mat_size, mat_size, Mul);
    ADDTIME_COR(MatPerformOper, div_time, cycles, mat1, mat2, out, mat_size, mat_size, Div);
  }

  printf("Serial execution:\n\tSum:\t%f\n\tDif:\t%f\n\tMul:\t%f\n\tDiv:\t%f\n",
         sum_time, dif_time, mul_time, div_time);

  MatDeinit(mat1, mat_size);
  MatDeinit(mat2, mat_size);
  MatDeinit(out, mat_size);
  #endif 

  return 0;
}
