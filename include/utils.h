#pragma once

#include <stdint.h>
#include <string.h>
#include "candefs.h"


void bitarray_to_str(char *dst, const uint8_t *src, const uint32_t size);
uint32_t crc15(const uint8_t *bitarray, const uint32_t size);