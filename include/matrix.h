#pragma once
#ifndef MATRIX_H

#include "config.h"

#include <stdint.h>

void MatInit(arr_t** mat, const uint32_t size);
void MatDeinit(arr_t** mat, const uint32_t size);

void MatPerformOper(arr_t** mat1, arr_t** mat2, arr_t** out, const uint32_t size,
                 void* op(arr_t*, arr_t*, arr_t*, const uint32_t));

#endif // !MATRIX_H
