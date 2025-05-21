#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define ARRAY_SIZE (uint32_t)atoi(getenv("ARRAY_SIZE"))
#define CYCLES (uint32_t)atoi(getenv("CYCLES"))

#define arr_t uint8_t

#define MIN_RAND 1
#define MAX_RAND 255

#endif // !CONFIG_H
