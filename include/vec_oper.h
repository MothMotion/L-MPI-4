#pragma once
#ifndef VEC_OPER_H
#define VEC_OPER_H

#include "config.h"
#include "random.h"

#include <stdint.h>
#include <stddef.h>

#include <mpi.h>

void Sum(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);
void Dif(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);
void Mul(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);
void Div(arr_t* arr1, arr_t* arr2, arr_t* arr_out, const uint32_t size);

#endif // !VEC_OPER_H
