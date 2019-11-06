#include <stdint.h>
#include "utils.h"


uint32_t crc15(uint8_t *bitarray, uint32_t size) {
    // uint8_t res[15] = {0};
    uint8_t crc[15] = {0};

    for(uint32_t i = 0; i < size; ++i) {
        uint8_t invert = bitarray[i] ^ crc[14];
        crc[14]        = crc[13] ^ invert;
        crc[13]        = crc[12];
        crc[12]        = crc[11];
        crc[11]        = crc[10];
        crc[10]        = crc[9] ^ invert;
        crc[9]         = crc[8];
        crc[8]         = crc[7] ^ invert;
        crc[7]         = crc[6] ^ invert;
        crc[6]         = crc[5];
        crc[5]         = crc[4];
        crc[4]         = crc[3] ^ invert;
        crc[3]         = crc[2] ^ invert;
        crc[2]         = crc[1];
        crc[1]         = crc[0];
        crc[0]         = invert;
    }
    uint32_t res = 0x0;
    for(uint32_t i = 0; i < 15; ++i) {
        res |= crc[i] << i;
    }
    return res;
}

// void byte_to_binary(uint8_t *dst, uint8_t src) {
//     for(uint8_t i = 0; i < 8; ++i) {
//         dst[i] = src & (1 << i);
//     }
// }

// static void reverse_array(uint8_t *dst, uint8_t *src, uint32_t size) {
//     uint8_t r[256] = {0};
//     for(uint32_t i = 0; i < size; ++i) {
//         r[i] = src[size - i - 1];
//     }
//     memcpy(dst, src, size);
// }