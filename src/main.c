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
  const uint32_t arr_size = ARRAY_SIZE;
  const uint32_t cycles   = CYCLES;

  arr_t **mat1 = malloc(arr_size * sizeof(arr_t*)),
        **mat2 = malloc(arr_size * sizeof(arr_t*)),
        **out  = malloc(arr_size * sizeof(arr_t*));
  MatInit(mat1, arr_size);
  MatInit(mat2, arr_size);
  MatInit(out, arr_size);

  float sum_time = 0.0, dif_time = 0.0,
        mul_time = 0.0, div_time = 0.0; 
 


  #ifndef SERIAL
  MPI_Init(&argc, &argv);
  int32_t rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  uint32_t width = arr_size / size + (arr_size % size) ? 1 : 0;
  
  if(rank == 0)
    printf("Settings:\n\tARRAY_SIZE:\t%d\n\tCYCLES:\t%d\n\tElement size:\t%lu\n\tCORES_AMOUNT:\t%d\n",
           arr_size, cycles, sizeof(arr_t), size);

  for(uint32_t i=0; i<cycles; ++i) {
    if(rank == 0)
      RandMat(mat1, arr_size, MIN_RAND, MAX_RAND);

    if(rank == 1)
      RandMat(mat2, arr_size, MIN_RAND, MAX_RAND);

    MPI_BROADCAST(mat1, arr_size, rank, 0, size);
    MPI_BROADCAST(mat2, arr_size, rank, 1, size);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_TIME(MatPerformOper, sum_time, cycles, rank, mat1, mat2, out, arr_size, Sum);
    ADDTIME_COR(MPI_SYNC, sum_time, cycles, out, arr_size, rank, size, width);

    MPI_TIME(MatPerformOper, dif_time, cycles, rank, mat1, mat2, out, arr_size, Dif);
    ADDTIME_COR(MPI_SYNC, dif_time, cycles, out, arr_size, rank, size, width);

    MPI_TIME(MatPerformOper, mul_time, cycles, rank, mat1, mat2, out, arr_size, Mul);
    ADDTIME_COR(MPI_SYNC, mul_time, cycles, out, arr_size, rank, size, width);

    MPI_TIME(MatPerformOper, div_time, cycles, rank, mat1, mat2, out, arr_size, Div);
    ADDTIME_COR(MPI_SYNC, div_time, cycles, out, arr_size, rank, size, width);
  }
  MPI_Finalize();
  if(rank == 0)
    printf("Parallel execution:\n\tSum:\t%f\n\tDif:\t%f\n\tMul:\t%f\n\tDiv:\t%f\n",
         sum_time, dif_time, mul_time, div_time);

  #else
  printf("Settings:\n\tARRAY_SIZE:\t%d\n\tCYCLES:\t%d\n\tElement size:\t%lu\n",
         arr_size, cycles, sizeof(arr_t));

  for(uint32_t i=0; i<cycles; ++i) {
    RandMat(mat1, arr_size, MIN_RAND, MAX_RAND);
    RandMat(mat2, arr_size, MIN_RAND, MAX_RAND);

    ADDTIME_COR(MatPerformOper, sum_time, cycles, mat1, mat2, out, arr_size, Sum);
    ADDTIME_COR(MatPerformOper, dif_time, cycles, mat1, mat2, out, arr_size, Dif);
    ADDTIME_COR(MatPerformOper, mul_time, cycles, mat1, mat2, out, arr_size, Mul);
    ADDTIME_COR(MatPerformOper, div_time, cycles, mat1, mat2, out, arr_size, Div);
  }

  printf("Serial execution:\n\tSum:\t%f\n\tDif:\t%f\n\tMul:\t%f\n\tDiv:\t%f\n",
         sum_time, dif_time, mul_time, div_time);

  #endif

  MatDeinit(mat1, arr_size);
  MatDeinit(mat2, arr_size);
  MatDeinit(out, arr_size);

  return 0;
}
